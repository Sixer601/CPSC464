all: networkDaemon parallelMergeSort

parallelMergeSort:	parallelMergeSort.o ServerSocket.o ClientSocket.o HelperFunctions.o Socket.o IntArray.o
	g++ parallelMergeSort.o ServerSocket.o ClientSocket.o Socket.o HelperFunctions.o IntArray.o -o parallelMergeSort

parallelMergeSort.o:	ServerSocket.h ClientSocket.h SocketException.h IntArray.h HelperFunctions.h Constants.h parallelMergeSort.cc
	g++ -c parallelMergeSort.cc

networkDaemon:	networkDaemon.o ConnectionList.o ServerSocket.o ClientSocket.o Socket.o
	g++ networkDaemon.o ConnectionList.o ServerSocket.o ClientSocket.o Socket.o -o networkDaemon

networkDaemon.o:	ConnectionList.h ServerSocket.h ClientSocket.h SocketException.h networkDaemon.cc
	g++ -c networkDaemon.cc

ServerSocket.o:	Socket.h SocketException.h ServerSocket.h ServerSocket.cc
	g++ -c ServerSocket.cc

ClientSocket.o:	Socket.h SocketException.h ClientSocket.h ClientSocket.cc
	g++ -c ClientSocket.cc

Socket.o:	Socket.h SocketException.h Socket.cc
	g++ -c Socket.cc

ConnectionList.o:	ConnectionList.h ConnectionList.cc
	g++ -c ConnectionList.cc

IntArray.o:	IntArray.h IntArray.cc
	g++ -c IntArray.cc

HelperFunctions.o:	HelperFunctions.h HelperFunctions.cc
	g++ -c HelperFunctions.cc

clean:
	rm -f *.o networkDaemon