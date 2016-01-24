CC = g++
CFLAGS =-g -Wall -std=c++0x -fPIC

all: ParserBase.o TitleParser.o AbstractParser.o KeywordsParser.o PubDateParser.o AuthorParser.o ReferencesParser.o CitationParser.o MetadataParser.o BodyParser.o PubmedXMLParser.o

ParserBase.o: ParserBase.cpp ParserBase.h
	$(CC) $(CFLAGS) -c ParserBase.cpp -o ParserBase.o

TitleParser.o: TitleParser.cpp TitleParser.h
	$(CC) $(CFLAGS) -c TitleParser.cpp -o TitleParser.o

AbstractParser.o: AbstractParser.cpp AbstractParser.h
	$(CC) $(CFLAGS) -c AbstractParser.cpp -o AbstractParser.o

KeywordsParser.o: KeywordsParser.cpp KeywordsParser.h
	$(CC) $(CFLAGS) -c KeywordsParser.cpp -o KeywordsParser.o

PubDateParser.o: PubDateParser.cpp PubDateParser.h
	$(CC) $(CFLAGS) -c PubDateParser.cpp -o PubDateParser.o

AuthorParser.o: AuthorParser.cpp AuthorParser.h
	$(CC) $(CFLAGS) -c AuthorParser.cpp -o AuthorParser.o

ReferencesParser.o: ReferencesParser.cpp ReferencesParser.h
	$(CC) $(CFLAGS) -c ReferencesParser.cpp -o ReferencesParser.o

CitationParser.o: CitationParser.cpp CitationParser.h
	$(CC) $(CFLAGS) -c CitationParser.cpp -o CitationParser.o

MetadataParser.o: MetadataParser.cpp MetadataParser.h
	$(CC) $(CFLAGS) -c MetadataParser.cpp -o MetadataParser.o

BodyParser.o: BodyParser.cpp BodyParser.h
	$(CC) $(CFLAGS) -c BodyParser.cpp -o BodyParser.o

PubmedXMLParser.o: PubmedXMLParser.cpp PubmedXMLParser.h
	$(CC) $(CFLAGS) -c PubmedXMLParser.cpp -o PubmedXMLParser.o

clean:
	rm -f *.o
	rm -f *~
