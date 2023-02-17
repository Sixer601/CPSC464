Homework3:	IntArray.o smbeardsley-homework3.o HelperFunctions.o
	g++ smbeardsley-homework3.o IntArray.o HelperFunctions.o -o Homework3

smbeardsley-homework3.o:	IntArray.h HelperFunctions.h smbeardsley-homework3.cc
	g++ -c smbeardsley-homework3.cc

HelperFunctions.o:	HelperFunctions.h HelperFunctions.cc
	g++ -c HelperFunctions.cc

IntArray.o:	IntArray.h IntArray.cc
	g++ -c IntArray.cc