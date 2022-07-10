#!/usr/bin/env python3
import socket
import json

HOST = 'localhost'
PORT = 8081
filename = "file.txt"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))

    s.sendall(json.dumps({
        "filename": filename
    }).encode('utf-8'))
    json_response = json.loads(s.recv(2048))
    size = json_response["filesize"]
    print(f"expected filesize {size}")
    s.sendall(b"ok")
    print("waiting file...")
    dl_file = s.recv(size)
    
    with open(f"downloads/{filename}", "w") as file:
        file.write(dl_file.decode())
        file.close()

    s.close()