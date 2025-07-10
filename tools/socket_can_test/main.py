import asyncio
import json
import os
from typing import Dict
from functools import reduce

# 导入自定义模块
from dbc_parser import DbcParser
from network_manager import NetworkManager
from config import get_config

# 导入cantools用于版本显示
import cantools
LOG_FILE = "can_batch.log"

print(f"正在使用的 cantools 版本 (Using cantools version): {cantools.__version__}")

# 新增：用于批量报文推送的全局队列
batch_send_queue = asyncio.Queue()

async def can_message_generator(parser: DbcParser, can_message_map: Dict, map_lock: asyncio.Lock, 
                               update_queue: asyncio.Queue, stop_event: asyncio.Event):
    """
    Task: Periodically generate CAN messages based on DBC cycle times and update can_message_map.
    """
    # 只为非诊断报文初始化消息映射
    async with map_lock:
        for message in parser.filtered_messages:
            message_id = message.frame_id
            # 跳过诊断报文 (已经在filtered_messages中被过滤了，但这里再次检查以确保安全)
            if parser._is_diagnostic_message(message_id):
                continue
            if message_id not in can_message_map:
                can_message_map[message_id] = {"data": b"", "signals": {}, "refresh_count": 0, "overrides": {}}
            elif "overrides" not in can_message_map[message_id]:
                can_message_map[message_id]["overrides"] = {}
            print(f"Initialized can_message_map for ID 0x{message_id:x}")

    tasks = []
    for message in parser.filtered_messages:
        message_id = message.frame_id
        if parser._is_diagnostic_message(message_id):
            continue
        cycle_time = parser.get_message_cycle_time(message_id)

        async def generate_for_message(msg_id: int, cycle: float):
            while not stop_event.is_set():
                async with map_lock:
                    msg_content = can_message_map.get(msg_id, {"signals": {}, "refresh_count": 0, "overrides": {}})
                    signal_values = msg_content["signals"]
                    overrides = msg_content.get("overrides", {})
                    changed = False
                    # 处理临时生效信号周期
                    for name in list(overrides.keys()):
                        info = overrides[name]
                        if info["remain_cycles"] > 0:
                            info["remain_cycles"] -= 1
                            print(f"Signal '{name}' of 0x{msg_id:x} refresh count: {msg_content['refresh_count']}, remain_cycles: {info['remain_cycles']}")
                            if info["remain_cycles"] == 0:
                                # 恢复为0（或origin_value）
                                old_value = signal_values.get(name, 0)
                                signal_values[name] = info.get("origin_value", 0)
                                print(f"Signal '{name}' of 0x{msg_id:x} auto restored from {old_value} to {signal_values[name]}")
                                del overrides[name]
                                changed = True
                    # 清理无用 overrides
                    if changed:
                        can_message_map[msg_id]["signals"] = signal_values
                        can_message_map[msg_id]["overrides"] = overrides
                    # 生成报文
                    try:
                        data, used_signals = parser.generate_message(
                            msg_id,
                            signal_values,
                            checksum_func=lambda data: reduce(lambda x, y: x ^ y, data[:-1], 0)
                        )
                        can_message_map[msg_id] = {
                            "data": data,
                            "signals": used_signals,
                            "refresh_count": can_message_map[msg_id]["refresh_count"] + 1,
                            "overrides": overrides
                        }
                    except ValueError as e:
                        print(f"Error generating message ID 0x{msg_id:x}: {e}")
                await asyncio.sleep(cycle)
        tasks.append(asyncio.create_task(generate_for_message(message_id, cycle_time)))

    # 处理更新队列
    while not stop_event.is_set():
        try:
            message_id, signals, cycles = await asyncio.wait_for(update_queue.get(), timeout=0.01)
            if parser._is_diagnostic_message(message_id):
                print(f"Warning: Ignoring update for diagnostic message ID 0x{message_id:x}")
                update_queue.task_done()
                continue
            async with map_lock:
                if message_id in can_message_map:
                    can_message_map[message_id]["signals"].update(signals)
                    can_message_map[message_id]["refresh_count"] += 1
                    # 保存覆盖周期
                    for name, cycle_num in (cycles or {}).items():
                        if cycle_num is not None:
                            origin_value = 0  # 可改为can_message_map[message_id]["signals"].get(name, 0)
                            can_message_map[message_id]["overrides"][name] = {
                                "remain_cycles": cycle_num,
                                "origin_value": origin_value
                            }
                    print(f"Updated signals for ID 0x{message_id:x} via queue, refresh_count: {can_message_map[message_id]['refresh_count']}")
            update_queue.task_done()
        except asyncio.TimeoutError:
            pass

    await asyncio.gather(*tasks, return_exceptions=True)
    print("CAN message generator stopped")

async def console_handler(parser: DbcParser, can_message_map: Dict, map_lock: asyncio.Lock, 
                         update_queue: asyncio.Queue, stop_event: asyncio.Event):
    """
    Task: Handle console input for editing signals and printing can_message_map.
    """
    print("\n=== 控制台命令帮助 ===")
    print("edit <id> <signal>=<value>[:<cycles>] [...]  - 编辑信号值，可用:cycles指定生效周期")
    print("print [id1 id2 ...]              - 打印消息映射（可选择特定ID）")
    print("print_cycle_times [id1 id2 ...]  - 打印周期时间（可选择特定ID）")
    print("debug <id>                       - 调试消息属性")
    print("summary                          - 打印消息摘要")
    print("server_info                      - 显示服务器信息")
    print("exit                             - 退出程序")
    print("help                             - 显示此帮助信息")
    print("=" * 50)

    while not stop_event.is_set():
        try:
            command = await asyncio.get_event_loop().run_in_executor(None, input, "> ")
            command = command.strip()

            if command.lower() == "exit":
                print("Received exit command, stopping all tasks...")
                stop_event.set()
                break
            elif command.lower() == "help":
                print("\n=== 控制台命令帮助 ===")
                print("edit <id> <signal>=<value>[:<cycles>] [...]  - 编辑信号值，可用:cycles指定生效周期")
                print("print [id1 id2 ...]              - 打印消息映射（可选择特定ID）")
                print("print_cycle_times [id1 id2 ...]  - 打印周期时间（可选择特定ID）")
                print("debug <id>                       - 调试消息属性")
                print("summary                          - 打印消息摘要")
                print("server_info                      - 显示服务器信息")
                print("exit                             - 退出程序")
                print("help                             - 显示此帮助信息")
                print("=" * 50)
            elif command.lower() == "summary":
                parser.print_message_summary()
            elif command.lower() == "server_info":
                print("Server running on 127.0.0.1:12345")
            elif command.lower().startswith("debug "):
                parts = command.split()
                if len(parts) >= 2:
                    try:
                        message_id = int(parts[1], 16) if parts[1].startswith("0x") else int(parts[1])
                        if parser._is_diagnostic_message(message_id):
                            print(f"Warning: Message ID 0x{message_id:x} is a diagnostic message and has been filtered out")
                        else:
                            parser.debug_message_attributes(message_id)
                    except ValueError:
                        print(f"Invalid message ID: {parts[1]}")
                else:
                    print("Usage: debug <message_id>")
            elif command.lower().startswith("print_cycle_times"):
                parts = command.split()
                async with map_lock:
                    print("\nCAN Message Cycle Times (Non-Diagnostic Messages Only):")
                    print(f"{'ID':<8} {'Cycle Time (ms)':<15} {'Cycle Time (s)'}")
                    print("-" * 50)
                    if len(parts) == 1:
                        for message in parser.filtered_messages:
                            message_id = message.frame_id
                            if parser._is_diagnostic_message(message_id):
                                continue
                            cycle_time = parser.get_message_cycle_time(message_id)
                            cycle_time_ms = cycle_time * 1000.0
                            print(f"0x{message_id:<6x} {cycle_time_ms:<15.2f} {cycle_time:.3f}")
                    else:
                        for part in parts[1:]:
                            try:
                                message_id = int(part, 16) if part.startswith("0x") else int(part)
                                if parser._is_diagnostic_message(message_id):
                                    print(f"Warning: Message ID 0x{message_id:x} is a diagnostic message and has been filtered out")
                                    continue
                                cycle_time = parser.get_message_cycle_time(message_id)
                                cycle_time_ms = cycle_time * 1000.0
                                print(f"0x{message_id:<6x} {cycle_time_ms:<15.2f} {cycle_time:.3f}")
                            except ValueError:
                                print(f"Invalid message ID: {part}")
            elif command.lower().startswith("print"):
                parts = command.split()
                async with map_lock:
                    print("\nCAN Message Map (Non-Diagnostic Messages Only):")
                    print(f"{'ID':<8} {'Hex Data':<20} {'Refresh Count':<15} {'Signals'}")
                    print("-" * 100)
                    if len(parts) == 1:
                        for msg_id, content in can_message_map.items():
                            if parser._is_diagnostic_message(msg_id):
                                continue
                            hex_data = content["data"].hex() if content["data"] else "N/A"
                            refresh_count = content["refresh_count"]
                            signals_str = ", ".join(f"{k}: {v}" for k, v in content["signals"].items()) or "No signals"
                            print(f"0x{msg_id:<6x} {hex_data:<20} {refresh_count:<15} {signals_str}")
                    else:
                        for part in parts[1:]:
                            try:
                                message_id = int(part, 16) if part.startswith("0x") else int(part)
                                if parser._is_diagnostic_message(message_id):
                                    print(f"Warning: Message ID 0x{message_id:x} is a diagnostic message and has been filtered out")
                                    continue
                                if message_id in can_message_map:
                                    content = can_message_map[message_id]
                                    hex_data = content["data"].hex() if content["data"] else "N/A"
                                    refresh_count = content["refresh_count"]
                                    signals_str = ", ".join(f"{k}: {v}" for k, v in content["signals"].items()) or "No signals"
                                    print(f"0x{message_id:<6x} {hex_data:<20} {refresh_count:<15} {signals_str}")
                                else:
                                    print(f"Message ID 0x{message_id:x} not found in CAN message map")
                            except ValueError:
                                print(f"Invalid message ID: {part}")
            elif command.startswith("edit "):
                parts = command.split()
                if len(parts) < 3:
                    print("Invalid edit command. Use: edit <id> <signal>=<value>[:<cycles>] [...]")
                    continue
                try:
                    message_id = int(parts[1], 16) if parts[1].startswith("0x") else int(parts[1])
                    if parser._is_diagnostic_message(message_id):
                        print(f"Error: Message ID 0x{message_id:x} is a diagnostic message and has been filtered out")
                        continue
                    signal_values = {}
                    signal_cycles = {}
                    for pair in parts[2:]:
                        if '=' not in pair:
                            print(f"Invalid signal format: {pair}")
                            continue
                        name, value_cycle = pair.split('=', 1)
                        if ':' in value_cycle:
                            value_str, cycle_str = value_cycle.split(':', 1)
                            try:
                                value = float(value_str)
                                cycles = int(cycle_str)
                            except ValueError:
                                print(f"Invalid value or cycles: {value_cycle}")
                                continue
                        else:
                            try:
                                value = float(value_cycle)
                                cycles = None
                            except ValueError:
                                print(f"Invalid value for {name}: {value_cycle}")
                                continue
                        signal_values[name] = value
                        signal_cycles[name] = cycles
                    if signal_values:
                        try:
                            parser.validate_signal_values(message_id, signal_values)
                        except ValueError as e:
                            print(f"Error: Invalid signal values: {e}")
                            continue
                        async with map_lock:
                            if message_id not in can_message_map:
                                can_message_map[message_id] = {"data": b"", "signals": {}, "refresh_count": 0, "overrides": {}}
                            can_message_map[message_id]["signals"].update(signal_values)
                            # 本地直接覆盖overrides（为兼容并发，队列传递更优）
                            for name, cycles in signal_cycles.items():
                                if cycles is not None:
                                    can_message_map[message_id]["overrides"][name] = {
                                        "remain_cycles": cycles,
                                        "origin_value": 0  # 你也可以改成原始值
                                    }
                            can_message_map[message_id]["refresh_count"] += 1
                            await update_queue.put((message_id, signal_values, signal_cycles))
                        print(f"Updated signals for ID 0x{message_id:x}: {signal_values} with cycles: {signal_cycles}")
                except ValueError as e:
                    print(f"Error in edit command: {e}")
            else:
                if command:
                    print("Unknown command. Type 'help' for available commands.")
        except Exception as e:
            print(f"Console error: {e}")
        await asyncio.sleep(0.1)
    print("Console handler stopped")

async def can_message_batch_logger(
    can_message_map: dict, map_lock: asyncio.Lock, stop_event: asyncio.Event,
    log_file: str = "can_batch.log", batch_send_queue: asyncio.Queue = None
):
    """
    每2毫秒遍历can_message_map，批量收集有刷新（refresh_count > 0）的CAN报文，
    以 0x02|CANID(2B)|长度(1B)|数据 格式拼接，最终整体包头加 0x02,0x01,0x01,长度(2B,小端)，尾部加异或校验。
    单包总长不超250字节，写入日志文件，并推送到batch_send_queue。
    
    新增逻辑：
    - 保持原有的 refresh_count > 0 逻辑不变
    - 如果某个报文的数据内容前后没有变化，则每10次处理一次该报文
    - 如果报文数据内容有变化，则立即处理该报文
    """
    def xor_checksum(data: bytes) -> int:
        checksum = 0
        for b in data:
            checksum ^= b
        return checksum

    # 用于记录每个报文的上一次数据状态和计数器
    last_msg_data = {}  # {msg_id: last_data}
    no_change_counters = {}  # {msg_id: counter}
    
    try:
        with open(log_file, "ab") as f:
            while not stop_event.is_set():
                await asyncio.sleep(0.002)
                packets = []
                
                async with map_lock:
                    items = list(can_message_map.items())
                    
                    for msg_id, content in items:
                        # 只处理有刷新的报文（保持原有逻辑）
                        if content.get("refresh_count", 0) > 0 and content.get("data"):
                            can_data = content["data"]
                            if not can_data:
                                continue
                                
                            # 检查该报文数据是否有变化
                            has_changed = True
                            if msg_id in last_msg_data and last_msg_data[msg_id] == can_data:
                                # 数据没有变化
                                has_changed = False
                                no_change_counters[msg_id] = no_change_counters.get(msg_id, 0) + 1
                            else:
                                # 数据有变化，重置计数器
                                no_change_counters[msg_id] = 0
                                last_msg_data[msg_id] = can_data
                            
                            # 决定是否处理该报文
                            should_process = False
                            if has_changed:
                                # 数据有变化，立即处理
                                should_process = True
                                print(f"[BatchLogger] 0x{msg_id:x} data changed, processing immediately")
                            elif no_change_counters[msg_id] >= 10:
                                # 数据没变化但计数器达到10，处理并重置计数器
                                should_process = True
                                no_change_counters[msg_id] = 0
                                print(f"[BatchLogger] 0x{msg_id:x} no change for 10 cycles, processing")
                            
                            if should_process:
                                can_id_bytes = msg_id.to_bytes(2, "big")
                                can_len = len(can_data)
                                if can_len > 255:
                                    print(f"[BatchLogger] Warning: CAN data too long for 0x{msg_id:x}, skipped")
                                    continue
                                packet = bytearray()
                                packet.append(0x02)
                                packet.extend(can_id_bytes)
                                packet.append(can_len)
                                packet.extend(can_data)
                                packets.append(packet)
                                # 清除刷新状态
                                content["refresh_count"] = 0
                            else:
                                # 不处理该报文，但仍需清除刷新状态以避免下次重复处理
                                content["refresh_count"] = 0

                # 拼装总报文（<=250字节）
                if packets:
                    payload = bytearray()
                    for pkt in packets:
                        # 预估加上头部/长度/校验不会超过250
                        if len(payload) + len(pkt) > 240:  # 预留10字节头尾，实际可适当再放宽
                            break
                        payload.extend(pkt)

                    if payload:
                        # 头部: 0x02,0x01,0x01,长度(2B，小端)
                        head = bytearray([0x02, 0x01, 0x01])
                        total_len = len(payload)
                        head.extend(total_len.to_bytes(2, "little"))  # 小端序
                        out_buf = head + payload
                        # 校验
                        checksum = xor_checksum(out_buf)
                        out_buf.append(checksum)
                        # 写入日志
                        f.write(out_buf.hex().encode() + b'\n')
                        f.flush()
                        # 推送到队列，供socket服务端发送
                        if batch_send_queue is not None:
                            await batch_send_queue.put(bytes(out_buf))
                            
    except Exception as e:
        print(f"[BatchLogger] Error: {e}")
    print("[BatchLogger] Task stopped, log file saved.")

async def batch_sender_task(batch_send_queue: asyncio.Queue, connected_clients: set, stop_event: asyncio.Event):
    """
    从batch_send_queue取出打包报文，推送给所有已连接客户端。
    connected_clients为set，存储writer对象。
    """
    while not stop_event.is_set():
        try:
            batch = await batch_send_queue.get()
            to_remove = set()
            for writer in connected_clients:
                try:
                    writer.write(batch)
                    await writer.drain()
                except Exception as e:
                    print(f"[BatchSender] Remove disconnected client: {e}")
                    to_remove.add(writer)
            # 移除失效连接
            for writer in to_remove:
                connected_clients.discard(writer)
        except Exception as e:
            print(f"[BatchSender] Error: {e}")
        await asyncio.sleep(0)

# 退出前查看日志内容
def print_log_file(log_file=LOG_FILE):
    if not os.path.exists(log_file):
        print(f"Log file {log_file} does not exist.")
        return
    print("\n=== CAN Batch Log File Content ===")
    with open(log_file, "r") as f:
        for idx, line in enumerate(f, 1):
            print(f"Batch {idx}: {line.strip()}")
    print("=== End of Log ===")

async def main():
    print("=== CAN Message Generator System ===")
    print("Starting system initialization...")

    dbc_config = get_config("dbc")
    dbc_path = dbc_config["dbc_path"]
    try:
        parser = DbcParser(dbc_path)
        print("✓ DBC parser initialized successfully")
    except Exception as e:
        print(f"✗ Failed to initialize DBC parser: {e}")
        return

    parser.print_message_summary()

    network_config = get_config("network")
    try:
        network_manager = NetworkManager(
            host=network_config["host"], 
            port=network_config["port"]
        )
        print("✓ Network manager initialized successfully")
    except Exception as e:
        print(f"✗ Failed to initialize network manager: {e}")
        return

    can_message_map = {}
    map_lock = asyncio.Lock()
    update_queue = asyncio.Queue()
    stop_event = asyncio.Event()

    print("\n=== Starting System Components ===")

    tasks = []

    # 新增：维护所有已连接客户端writer对象
    connected_clients = set()
    # 修改socket server，使其接入connected_clients
    server_task = asyncio.create_task(
        network_manager.start_socket_server(
            parser, can_message_map, map_lock, update_queue, stop_event, connected_clients
        ),
        name="NetworkServer"
    )
    tasks.append(server_task)

    generator_task = asyncio.create_task(
        can_message_generator(
            parser, can_message_map, map_lock, update_queue, stop_event
        ),
        name="MessageGenerator"
    )
    tasks.append(generator_task)

    console_task = asyncio.create_task(
        console_handler(
            parser, can_message_map, map_lock, update_queue, stop_event
        ),
        name="ConsoleHandler"
    )
    tasks.append(console_task)

    # 添加批量日志记录任务。传入batch_send_queue
    batch_logger_task = asyncio.create_task(
        can_message_batch_logger(can_message_map, map_lock, stop_event, batch_send_queue=batch_send_queue),
        name="BatchLogger"
    )
    tasks.append(batch_logger_task)

    # 批量报文推送任务
    batch_sender = asyncio.create_task(
        batch_sender_task(batch_send_queue, connected_clients, stop_event),
        name="BatchSender"
    )
    tasks.append(batch_sender)

    print("✓ All system components started")
    print("\n=== System Ready ===")
    print("You can now:")
    print("- Send TCP requests to 127.0.0.1:12345")
    print("- Use console commands (type 'help' for available commands)")
    print("- Type 'exit' to shutdown the system")
    print("=" * 50)

    try:
        await asyncio.gather(*tasks, return_exceptions=True)
    except KeyboardInterrupt:
        print("\n\nReceived keyboard interrupt, shutting down...")
        stop_event.set()
        for task in tasks:
            if not task.done():
                task.cancel()
        await asyncio.gather(*tasks, return_exceptions=True)
    #finally:
    #    print_log_file(LOG_FILE)

    print("\n=== System Shutdown Complete ===")

if __name__ == "__main__":
    try:
        import cantools
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nProgram interrupted by user")
    except Exception as e:
        print(f"Fatal error: {e}")
        import traceback
        traceback.print_exc()
