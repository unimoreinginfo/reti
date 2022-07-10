#!/usr/bin/env python3.9

import os
import socket
import signal
import json

HOST = 'localhost'
PORT = 8081

queue = dict()

def handle_request(filename):
    try:
        file = open(filename, "r").read()
        filesize = os.path.getsize(filename)
        return (filename, filesize, file)
    except(FileNotFoundError) as e:
        print(e)
        return None

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        
        s.bind((HOST, PORT))
        s.listen()

        def handle_sigint(signum, frame):
            s.close()
            exit(0)

        signal.signal(signal.SIGINT, handle_sigint)
        print("server listening on {}:{}".format(HOST, PORT))

        while True:
            conn, addr = s.accept()
            ip_port = f"{addr[0]}:{addr[1]}"
            data = conn.recv(1000)
            file_request = data.decode('utf-8')
            print(file_request)
            
            parsed = json.loads(file_request)
            try:
                filename = parsed["filename"]
                if(filename is None):
                    conn.sendall("'filename' cannot be empty")
                result = handle_request(filename)
                if(result is None):
                    conn.sendall(b"file not found")
                else:
                    (filename, filesize, data) = result

                    jsonified = json.dumps({
                        "filename": filename,
                        "filesize": filesize
                    }, indent=3)

                    conn.sendall(jsonified.encode('utf-8'))
                    queue[ip_port] = data
                    ok_packet = conn.recv(100).decode('utf-8')
                    if(ok_packet == "ok"):
                        conn.sendall(data.encode('utf-8'))
                        conn.close()

            except Exception as e:
                print(e)
                conn.sendall(b"fatal error occurred")
                conn.close()

            continue
    
