import asyncio

#SERVER_HOST = '127.0.0.1'
#SERVER_HOST = '172.26.119.223'
SERVER_HOST = '172.26.10.177'
#SERVER_HOST = '172.26.119.223'
SERVER_PORT = 12345
LOG_FILE = 'client_can_recv.log'

async def save_to_log(data: bytes, log_file: str):
    with open(log_file, 'ab') as f:
        f.write(data.hex().encode() + b'\n')
        f.flush()

async def can_client(host: str, port: int, log_file: str):
    try:
        reader, writer = await asyncio.open_connection(host, port)
        print(f'Connected to {host}:{port}. Receiving data...')
        while True:
            data = await reader.read(4096)
            if not data:
                print('Server closed connection.')
                break
            await save_to_log(data, log_file)
            print(f"Received {len(data)} bytes, saved to {log_file}")
    except KeyboardInterrupt:
        print("\nKeyboardInterrupt, client will exit.")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        print("Client stopped.")

if __name__ == '__main__':
    try:
        asyncio.run(can_client(SERVER_HOST, SERVER_PORT, LOG_FILE))
    except KeyboardInterrupt:
        print("\nClient stopped by user.")
