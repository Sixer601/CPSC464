all:	routerprog deviceprog

routerprog:	routerprog.o HelperFunctions.o Address.o Socket.o ConnectionsList.o Point.o RoutingTable.o Message.o
	g++ routerprog.o HelperFunctions.o Address.o Socket.o ConnectionsList.o Point.o RoutingTable.o Message.o -o routerprog

deviceprog:	deviceprog.o Socket.o HelperFunctions.o  Point.o Address.o RoutingTable.o Message.o
	g++ deviceprog.o Socket.o HelperFunctions.o Point.o Address.o RoutingTable.o Message.o -o deviceprog

routerprog.o:	Address.h Socket.h SocketException.h Constants.h HelperFunctions.h ConnectionsList.h Point.h RoutingTable.h Message.h routerprog.cc
	g++ -c routerprog.cc

deviceprog.o:	Socket.h SocketException.h Constants.h HelperFunctions.h Point.h Address.h RoutingTable.h Message.h deviceprog.cc
	g++ -c deviceprog.cc

Address.o:	Address.h Constants.h Address.cc
	g++ -c Address.cc

ConnectionsList.o:	Socket.h SocketException.h Constants.h ConnectionsList.h ConnectionsList.cc
	g++ -c ConnectionsList.cc

HelperFunctions.o:	HelperFunctions.h Constants.h HelperFunctions.cc
	g++ -c HelperFunctions.cc

Message.o:	Message.h Point.h Address.h RoutingTable.h Constants.h Message.cc
	g++ -c Message.cc

Point.o:	Point.h Constants.h Point.cc
	g++ -c Point.cc

RoutingTable.o: RoutingTable.h Address.h Constants.h RoutingTable.cc
	g++ -c RoutingTable.cc

Socket.o:	Socket.h SocketException.h Socket.cc
	g++ -c Socket.cc

clean:
	rm -f *.o routerprog deviceprog routerIP.txt
