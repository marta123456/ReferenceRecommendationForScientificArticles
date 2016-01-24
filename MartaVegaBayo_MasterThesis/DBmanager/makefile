CC = g++
CFLAGS =-g -Wall -std=c++0x -fPIC

all: DBBasic.o DBLowLevelManager.o DBManager.o


DBBasic.o: DBBasic.cpp DBBasic.h
	$(CC) $(CFLAGS) $(mysql_config --cflags) -c DBBasic.cpp $(mysql_config --libs)

DBLowLevelManager.o: DBLowLevelManager.cpp DBLowLevelManager.h
	$(CC) $(CFLAGS) $(mysql_config --cflags) -c DBLowLevelManager.cpp  $(mysql_config --libs)


DBManager.o: DBManager.cpp DBManager.h
	$(CC) $(CFLAGS) $(mysql_config --cflags) -c DBManager.cpp $(mysql_config --libs)

clean:
	rm -f *.o
	rm -f *~
	
