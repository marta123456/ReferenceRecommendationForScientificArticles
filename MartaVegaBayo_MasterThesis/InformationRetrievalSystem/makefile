CC = g++
CFLAGS =-g -Wall -std=c++0x -fPIC

all: IR.o IRTitleBased.o IRCitationBased.o IRTester.o

IR.o: IR.cpp IR.h
	$(CC) $(CFLAGS) $(mysql_config --cflags)  -c IR.cpp $(mysql_config --libs)

IRTitleBased.o: IRTitleBased.cpp IRTitleBased.h
	$(CC) $(CFLAGS) $(mysql_config --cflags) -c IRTitleBased.cpp $(mysql_config --libs)

IRCitationBased.o: IRCitationBased.cpp IRCitationBased.h
	$(CC) $(CFLAGS) $(mysql_config --cflags) -c IRCitationBased.cpp $(mysql_config --libs)

IRTester.o: IRTester.cpp IRTester.h
	$(CC) $(CFLAGS) $(mysql_config --cflags) -c IRTester.cpp $(mysql_config --libs)
clean:
	rm -f *.o
	rm -f *~
