# -*- coding: utf-8 -*-
import socket
import iot_parser

passive_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
passive_socket.bind((socket.gethostname(), 2000))
passive_socket.listen(1)


while True:
    active_socket, client_address = passive_socket.accept()
    print('Got connection from', client_address)

    while True:
        new_data = active_socket.recv(128)
        #print(new_data)
        if not new_data:
            break
        else:
            iot_parser.parse(new_data)
    active_socket.close()
    print("Socket closed.")
    break
