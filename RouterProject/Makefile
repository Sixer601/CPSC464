all:	routerprog deviceprog

routerprog:	routerprog.o HelperFunctions.o Address.o Socket.o ConnectionsList.o Point.o RoutingTable.o Message.o
	g++ routerprog.o HelperFunctions.o Address.o Socket.o ConnectionsList.o Point.o RoutingTable.o Message.o -o routerprog

deviceprog:	deviceprog.o Socket.o HelperFunctions.o  Point.o Address.o RoutingTable.o Device.o Message.o MessageList.o
	g++ deviceprog.o Socket.o HelperFunctions.o Point.o Address.o RoutingTable.o Device.o Message.o MessageList.o -o deviceprog

routerprog.o:	Address.h Socket.h SocketException.h Constants.h HelperFunctions.h ConnectionsList.h Point.h RoutingTable.h Message.h routerprog.cc
	g++ -c routerprog.cc

deviceprog.o:	Socket.h SocketException.h Constants.h HelperFunctions.h Point.h Address.h RoutingTable.h Device.h Message.h MessageList.h deviceprog.cc
	g++ -c deviceprog.cc

Address.o:	Address.h Constants.h Address.cc
	g++ -c Address.cc

ConnectionsList.o:	Socket.h SocketException.h Constants.h ConnectionsList.h ConnectionsList.cc
	g++ -c ConnectionsList.cc

Device.o:	Device.h Constants.h Address.h Point.h RoutingTable.h Device.cc
	g++ -c Device.cc

HelperFunctions.o:	HelperFunctions.h Constants.h HelperFunctions.cc
	g++ -c HelperFunctions.cc

Message.o:	Message.h Point.h Address.h RoutingTable.h Constants.h Message.cc
	g++ -c Message.cc

MessageList.o:	MessageList.h Message.h MessageList.cc
	g++ -c MessageList.cc

Point.o:	Point.h Constants.h Point.cc
	g++ -c Point.cc

RoutingTable.o: RoutingTable.h Address.h Constants.h RoutingTable.cc
	g++ -c RoutingTable.cc

Socket.o:	Socket.h SocketException.h Socket.cc
	g++ -c Socket.cc

clean:
	rm -f *.o routerprog deviceprog
