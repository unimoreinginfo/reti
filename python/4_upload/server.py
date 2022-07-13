#!/usr/bin/env python3

import socket
import time
import json
from os import path

HOST = '127.0.0.1'
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))

    print("Listening on %s:%d..." % (HOST, PORT))
    s.listen()

    while True:
        conn, addr = s.accept()

        # header
        header = ""
        while True:
            c = conn.recv(1).decode("utf-8")
            if c == "\n":
                break
            header += c

        header = json.loads(header)
        print(header)
        
        # write file
        dst_filename = path.join("uploads", header["filename"])

        print("Uploading file: " + dst_filename)

        f = open(dst_filename, "wb")
        f.write(conn.recv(header["filesize"]))
        f.close()

        # write response
        conn.sendall(json.dumps({ "statuscode": 200 }).encode("utf-8"))
