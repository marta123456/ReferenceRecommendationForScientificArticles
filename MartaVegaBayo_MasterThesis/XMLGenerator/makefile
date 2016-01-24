CC = g++
CFLAGS = -std=c++0x -g -Wall -fPIC

all: BasicGenerator.o  ArticlesXMLGenerator.o ExtendedArticlesXMLGenerator.o CitationsXMLGenerator.o XMLGenerator.o

BasicGenerator.o: BasicGenerator.cpp
	$(CC) $(CFLAGS) -c BasicGenerator.cpp -o BasicGenerator.o

ArticlesXMLGenerator.o: ArticlesXMLGenerator.cpp
	$(CC) $(CFLAGS) -c ArticlesXMLGenerator.cpp -o ArticlesXMLGenerator.o

ExtendedArticlesXMLGenerator.o: ExtendedArticlesXMLGenerator.cpp
	$(CC) $(CFLAGS) -c ExtendedArticlesXMLGenerator.cpp -o ExtendedArticlesXMLGenerator.o

CitationsXMLGenerator.o: CitationsXMLGenerator.cpp
	$(CC) $(CFLAGS) -c CitationsXMLGenerator.cpp -o CitationsXMLGenerator.o

XMLGenerator.o: XMLGenerator.cpp 
	$(CC) $(CFLAGS) -c XMLGenerator.cpp -o XMLGenerator.o




clean:
	rm -f *.o
	rm -f *~
