import asyncio
import json
from typing import Dict, Set

class NetworkManager:
    def __init__(self, host: str = '127.0.0.1', port: int = 12345):
        """
        Initialize Network Manager
        
        Args:
            host: Server host address
            port: Server port number
        """
        self.host = host
        self.port = port
        self.server = None

    async def start_socket_server(self, parser, can_message_map: Dict, map_lock: asyncio.Lock, 
                                 update_queue: asyncio.Queue, stop_event: asyncio.Event, connected_clients: Set = None):
        """
        Start TCP socket server to receive signal update requests from clients and push batch data to clients.

        Args:
            parser: DBC parser instance
            can_message_map: Shared CAN message map
            map_lock: Asyncio lock for thread-safe access
            update_queue: Queue for message updates
            stop_event: Event to signal server shutdown
            connected_clients: Set to keep track of connected client writers for batch push
        """
        if connected_clients is None:
            connected_clients = set()

        async def handle_client(reader: asyncio.StreamReader, writer: asyncio.StreamWriter):
            """
            Handle individual client connections and save received data to file
            """
            client_addr = writer.get_extra_info('peername')
            print(f"Client connected from {client_addr}")
            connected_clients.add(writer)
            try:
                while not stop_event.is_set():
                    data = await reader.read(1024)
                    if not data:
                        print(f"Client {client_addr} disconnected")
                        break
                    
                    # Save received data to file
                    try:
                        with open("received_data.txt", "ab") as f:
                            f.write(data)
                        print(f"Saved data from {client_addr} to file")
                        

                    
                    except Exception as e:
                        error_msg = f"Error: Failed to save data: {str(e)}\n"
                        writer.write(error_msg.encode())
                        print(f"Error saving data from {client_addr}: {str(e)}")
                        await writer.drain()
            
            except Exception as e:
                error_msg = f"Error: Server error: {str(e)}\n"
                try:
                    writer.write(error_msg.encode())
                    await writer.drain()
                except Exception:
                    pass
                print(f"Server error handling client {client_addr}: {str(e)}")
            finally:
                connected_clients.discard(writer)
                try:
                    writer.close()
                    await writer.wait_closed()
                    print(f"Client {client_addr} connection closed")
                except Exception as e:
                    print(f"Error closing connection to {client_addr}: {e}")

        try:
            self.server = await asyncio.start_server(handle_client, self.host, self.port)
            print(f"Socket server started on {self.host}:{self.port}")

            async with self.server:
                await stop_event.wait()

        except Exception as e:
            print(f"Error starting socket server: {e}")
            raise
        finally:
            if self.server:
                self.server.close()
                await self.server.wait_closed()
                print("Socket server stopped")

    async def send_client_request(self, message_id: int, signals: Dict[str, float]) -> str:
        """
        Send a request to the server as a client (for testing purposes)
        
        Args:
            message_id: CAN message ID
            signals: Dictionary of signal names and values
            
        Returns:
            Server response as string
        """
        try:
            reader, writer = await asyncio.open_connection(self.host, self.port)
            
            # 构造请求
            request = {
                "message_id": f"0x{message_id:x}",
                "signals": signals
            }
            
            # 发送请求
            request_str = json.dumps(request)
            writer.write(request_str.encode())
            await writer.drain()
            
            # 读取响应
            response = await reader.read(1024)
            response_str = response.decode().strip()
            
            # 关闭连接
            writer.close()
            await writer.wait_closed()
            
            return response_str
            
        except Exception as e:
            return f"Error: {str(e)}"

    def get_server_info(self) -> Dict[str, any]:
        """
        Get server information
        
        Returns:
            Dictionary containing server host, port, and status
        """
        return {
            "host": self.host,
            "port": self.port,
            "is_running": self.server is not None and not self.server.is_closed() if self.server else False
        }

    async def stop_server(self):
        """
        Stop the socket server
        """
        if self.server:
            self.server.close()
            await self.server.wait_closed()
            print("Socket server stopped manually")