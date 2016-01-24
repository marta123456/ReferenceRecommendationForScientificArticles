CC = g++
CFLAGS = -std=c++0x -g -Wall -fPIC

all: CName.o CDate.o CSection.o CArticle.o CReference.o CCitation.o CArticleParsed.o

CName.o: CName.cpp
	$(CC) $(CFLAGS) -c CName.cpp -o CName.o

CDate.o: CDate.cpp
	$(CC) $(CFLAGS) -c CDate.cpp -o CDate.o

CSection.o: CSection.cpp
	$(CC) $(CFLAGS) -c CSection.cpp -o CSection.o

CArticle.o: CArticle.cpp
	$(CC) $(CFLAGS) -c CArticle.cpp -o CArticle.o

CReference.o: CReference.cpp
	$(CC) $(CFLAGS) -c CReference.cpp -o CReference.o

CCitation.o: CCitation.cpp
	$(CC) $(CFLAGS) -c CCitation.cpp -o CCitation.o

CArticleParsed.o: CArticleParsed.cpp
	$(CC) $(CFLAGS) -c CArticleParsed.cpp -o CArticleParsed.o


clean:
	rm -f *.o
	rm -f *~ 
