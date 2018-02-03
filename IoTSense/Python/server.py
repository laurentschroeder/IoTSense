# -*- coding: utf-8 -*-
import socket
import iot_parser





passive_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
passive_socket.bind((socket.gethostname(), 2000))
passive_socket.listen(5)


while True:
    data = b''
    data_string = ''
    data_string_list = []
    active_socket, client_address = passive_socket.accept()
    print('Got connection from', client_address)

    while True:
        new_data = active_socket.recv(128)
        if not new_data:
            break
        else:
            #data += new_data
            #print("new_data:\t", len(new_data), "\t", new_data)
            #print("data:\t\t", len(data), "\t", data)
            iot_parser.parse(new_data)
            #print(new_data)
            #rint(new_data[1:3])
    active_socket.close()
    print("Socket closed.")
    break
