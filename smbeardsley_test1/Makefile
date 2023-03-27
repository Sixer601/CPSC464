all: networkDaemon parallelMergeSort

parallelMergeSort:	Socket.o HelperFunctions.o parallelMergeSort.o
	g++ Socket.o HelperFunctions.o parallelMergeSort.o -o parallelMergeSort

networkDaemon:	Socket.o networkDaemon.o 
	g++ Socket.o networkDaemon.o -o networkDaemon

networkDaemon.o:	Socket.h SocketException.h Constants.h networkDaemon.cc
	g++ -c networkDaemon.cc

parallelMergeSort.o:	Socket.h SocketException.h Constants.h HelperFunctions.h parallelMergeSort.cc
	g++ -c parallelMergeSort.cc

Socket.o:	Socket.h SocketException.h Socket.cc
	g++ -c Socket.cc

HelperFunctions.o:	HelperFunctions.cc
	g++ -c HelperFunctions.cc

clean:
	rm -f *.o networkDaemon parallelMergeSort
