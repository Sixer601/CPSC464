# Makefile for the socket programming example
#

all: simple_client_main simple_server_main

simple_client_main:	simple_client_main.o ClientSocket.o Socket.o
	g++ simple_client_main.o ClientSocket.o Socket.o -o simple_client_main

simple_server_main: simple_server_main.o ServerSocket.o Socket.o
	g++ simple_server_main.o ServerSocket.o Socket.o -o simple_server_main

simple_client_main.o:	ClientSocket.h SocketException.h simple_client_main.cc
	g++ -c simple_client_main.cc

simple_server_main.o:	ServerSocket.h SocketException.h simple_server_main.cc
	g++ -c simple_server_main.cc

ServerSocket.o:	Socket.h SocketException.h ServerSocket.h ServerSocket.cc
	g++ -c ServerSocket.cc

ClientSocket.o:	Socket.h SocketException.h ClientSocket.h ClientSocket.cc
	g++ -c ClientSocket.cc

Socket.o:	Socket.h SocketException.h Socket.cc
	g++ -c Socket.cc

clean:
	rm -f *.o simple_server simple_client