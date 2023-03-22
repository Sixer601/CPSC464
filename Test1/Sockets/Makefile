# Makefile for the socket programming example
#

simple_server_objects = ServerSocket.o Socket.o simple_server_main.o
simple_client_objects = ClientSocket.o Socket.o simple_client_main.o


all : simple_server simple_client

simple_server: $(simple_server_objects)
	g++ -o simple_server $(simple_server_objects)


simple_client: $(simple_client_objects)
	g++ -o simple_client $(simple_client_objects)


Socket: Socket.cc
ServerSocket: ServerSocket.cc
ClientSocket: ClientSocket.cc
simple_server_main: simple_server_main.cc
simple_client_main: simple_client_main.cc


clean:
	rm -f *.o simple_server simple_client
