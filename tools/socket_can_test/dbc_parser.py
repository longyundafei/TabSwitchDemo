import cantools
import json
from typing import Dict, List, Tuple, Optional, Callable
from functools import reduce

class DbcParser:
    def __init__(self, dbc_path: str):
        """
        Initialize DBC parser and load DBC file.
        """
        try:
            self.db = cantools.database.load_file(dbc_path)
            print(f"DBC file {dbc_path} loaded successfully")
            
            # 过滤掉诊断相关的报文 (0x7xx)
            self.filtered_messages = [m for m in self.db.messages if not self._is_diagnostic_message(m.frame_id)]
            total_messages = len(self.db.messages)
            filtered_count = len(self.filtered_messages)
            diagnostic_count = total_messages - filtered_count
            
            print(f"Total messages in DBC: {total_messages}")
            print(f"Diagnostic messages (0x7xx) filtered out: {diagnostic_count}")
            print(f"Remaining messages for processing: {filtered_count}")
            print(f"Loaded messages: {[hex(m.frame_id) for m in self.filtered_messages]}")
            
        except FileNotFoundError:
            raise FileNotFoundError(f"DBC file not found: {dbc_path}")
        except Exception as e:
            raise ValueError(f"Failed to load DBC file: {e}")
    
    def _is_diagnostic_message(self, frame_id: int) -> bool:
        """
        判断是否为诊断相关报文 (0x7xx范围)
        
        Args:
            frame_id: CAN报文ID
            
        Returns:
            bool: 如果是诊断报文返回True，否则返回False
        """
        # 诊断报文通常在0x700-0x7FF范围内
        return 0x700 <= frame_id <= 0x7FF

    def get_message_signals(self, message_id: int) -> List[Dict]:
        """
        Get all signals and their attributes for a specified message.
        """
        # 检查是否为诊断报文
        if self._is_diagnostic_message(message_id):
            raise ValueError(f"Message ID 0x{message_id:x} is a diagnostic message and has been filtered out")
            
        try:
            message = self.db.get_message_by_frame_id(message_id)
            signals = []
            for signal in message.signals:
                signals.append({
                    'name': signal.name,
                    'start': signal.start,
                    'length': signal.length,
                    'scale': signal.scale,
                    'offset': signal.offset,
                    'unit': signal.unit or '',
                    'min': signal.minimum if signal.minimum is not None else float('-inf'),
                    'max': signal.maximum if signal.maximum is not None else float('inf'),
                    'is_signed': signal.is_signed
                })
            return signals
        except KeyError:
            raise ValueError(f"Message with ID 0x{message_id:x} not found in DBC")

    def validate_signal_values(self, message_id: int, signal_values: Dict[str, float]) -> None:
        """
        Validate signal values for a message.
        """
        # 检查是否为诊断报文
        if self._is_diagnostic_message(message_id):
            raise ValueError(f"Message ID 0x{message_id:x} is a diagnostic message and has been filtered out")
            
        message = self.db.get_message_by_frame_id(message_id)
        valid_signals = {signal.name for signal in message.signals}
        
        for name, value in signal_values.items():
            if name not in valid_signals:
                raise ValueError(f"Invalid signal name: {name}")
            signal = next(s for s in message.signals if s.name == name)
            if signal.minimum is not None and value < signal.minimum:
                raise ValueError(f"Signal {name} value {value} below minimum {signal.minimum}")
            if signal.maximum is not None and value > signal.maximum:
                raise ValueError(f"Signal {name} value {value} above maximum {signal.maximum}")

    def generate_message(self, message_id: int, signal_values: Dict[str, float], 
                        checksum_func: Optional[Callable[[bytes], int]] = None) -> Tuple[bytes, Dict[str, float]]:
        """
        Generate message data for a given ID with specified signal values.
        Automatically detects and processes checksum signals (containing 'Chksum').
        """
        # 检查是否为诊断报文
        if self._is_diagnostic_message(message_id):
            raise ValueError(f"Message ID 0x{message_id:x} is a diagnostic message and has been filtered out")
            
        try:
            message = self.db.get_message_by_frame_id(message_id)
            self.validate_signal_values(message_id, signal_values)
            used_signals = {}
            
            for signal in message.signals:
                if signal.name in signal_values:
                    used_signals[signal.name] = signal_values[signal.name]
                else:
                    used_signals[signal.name] = signal.minimum if signal.minimum is not None else 0

            checksum_signal = next((s.name for s in message.signals if 'chksum' in s.name.lower()), None)
            
            if checksum_signal:
                temp_signals = used_signals.copy()
                temp_signals[checksum_signal] = 0
                try:
                    temp_data = self.db.encode_message(message.frame_id, temp_signals)
                    if checksum_func:
                        checksum = checksum_func(temp_data)
                    else:
                        checksum = reduce(lambda x, y: x ^ y, temp_data[:-1], 0)
                    used_signals[checksum_signal] = checksum
                except Exception as e:
                    raise ValueError(f"Failed to encode temporary data for checksum: {e}")

            data = self.db.encode_message(message.frame_id, used_signals)
            return data, used_signals
        except KeyError:
            raise ValueError(f"Message with ID 0x{message_id:x} not found in DBC")
        except Exception as e:
            raise ValueError(f"Failed to encode message: {e}")

    def get_message_cycle_time(self, message_id: int) -> float:
        """
        获取消息的周期时间（单位：秒），来自 DBC 中的 GenMsgCycleTime 属性。
        如果未指定或不可用，则返回默认值 0.1 秒（100ms）。
        """
        DEFAULT_CYCLE_TIME_SEC = 0.1
        
        # 检查是否为诊断报文
        if self._is_diagnostic_message(message_id):
            print(f"警告: 消息 ID 0x{message_id:x} 是诊断报文，已被过滤")
            return DEFAULT_CYCLE_TIME_SEC
    
        try:
            message = self.db.get_message_by_frame_id(message_id)
        except KeyError:
            print(f"错误: 在 DBC 中未找到消息 ID 0x{message_id:x}，使用默认值 {DEFAULT_CYCLE_TIME_SEC*1000:.0f}ms")
            return DEFAULT_CYCLE_TIME_SEC
        except Exception as e:
            print(f"获取消息时发生异常: {e}，使用默认值 {DEFAULT_CYCLE_TIME_SEC*1000:.0f}ms")
            return DEFAULT_CYCLE_TIME_SEC
    
        # 直接使用 cantools 提供的标准属性
        cycle_time_ms = getattr(message, "cycle_time", None)
    
        if cycle_time_ms is None:
            print(f"提示: 消息 ID 0x{message_id:x} 未设置 GenMsgCycleTime，使用默认值 {DEFAULT_CYCLE_TIME_SEC*1000:.0f}ms")
            return DEFAULT_CYCLE_TIME_SEC
    
        try:
            return float(cycle_time_ms) / 1000.0
        except (ValueError, TypeError) as e:
            print(f"警告: 消息 ID 0x{message_id:x} 的周期值无效 ({cycle_time_ms}): {e}，使用默认值 {DEFAULT_CYCLE_TIME_SEC*1000:.0f}ms")
            return DEFAULT_CYCLE_TIME_SEC

    def debug_message_attributes(self, message_id: int) -> None:
        """
        Debug function to print all available attributes for a message.
        """
        # 检查是否为诊断报文
        if self._is_diagnostic_message(message_id):
            print(f"警告: 消息 ID 0x{message_id:x} 是诊断报文，已被过滤")
            return
            
        try:
            message = self.db.get_message_by_frame_id(message_id)
            print(f"\n=== 调试消息 ID 0x{message_id:x} 的属性 ===")
            print(f"消息对象类型: {type(message)}")
            print(f"消息对象属性: {dir(message)}")
            
            # 检查消息对象的各种属性
            if hasattr(message, 'attributes'):
                print(f"message.attributes: {message.attributes}")
            if hasattr(message, 'attribute_values'):
                print(f"message.attribute_values: {message.attribute_values}")
            
            # 检查数据库的属性
            if hasattr(self.db, 'attributes'):
                print(f"数据库属性数量: {len(self.db.attributes) if self.db.attributes else 0}")
                if self.db.attributes:
                    for attr_name, attr_def in self.db.attributes.items():
                        if "cycle" in attr_name.lower() or "time" in attr_name.lower():
                            print(f"  相关属性: {attr_name} -> {attr_def}")
            
        except Exception as e:
            print(f"调试时发生错误: {e}")

    def get_non_diagnostic_messages(self) -> List[int]:
        """
        获取所有非诊断报文的ID列表
        
        Returns:
            List[int]: 非诊断报文ID列表
        """
        return [msg.frame_id for msg in self.filtered_messages]

    def print_message_summary(self) -> None:
        """
        打印消息摘要信息
        """
        non_diagnostic_messages = self.get_non_diagnostic_messages()
        print(f"\n=== 消息摘要 ===")
        print(f"非诊断报文数量: {len(non_diagnostic_messages)}")
        print(f"非诊断报文ID列表: {[hex(msg_id) for msg_id in non_diagnostic_messages]}")
