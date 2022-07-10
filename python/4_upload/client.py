#!/usr/bin/env python3

import socket
import sys
import json

# client.py <hostname> <filename>

HOST=sys.argv[1]
PORT=8080
FILENAME=sys.argv[2]

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))

    f = open(FILENAME, "rb")
    filedata = f.read()

    # upload
    msg = {
        "filename": FILENAME,
        "filesize": len(filedata)
    }
    s.sendall(json.dumps(msg).encode('utf-8'))
    s.sendall("\n".encode("utf-8"))
    s.sendall(filedata)
    
    # response
    resp = s.recv(1024).decode("utf-8")

    print("Server said:", json.loads(resp))
