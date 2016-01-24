#FLAGS AND CONSTANTS

#General compilation flags
CC = g++
CFLAGS =-g  -std=c++0x  -fPIC -std=gnu++0x

#Dependencies lists
PUGIXML_DEPS = ./Pugixml/pugixml.o

PARSER_DEPS = ./PubmedParser/ParserBase.o ./PubmedParser/PubmedXMLParser.o ./PubmedParser/MetadataParser.o ./PubmedParser/BodyParser.o ./PubmedParser/ReferencesParser.o ./PubmedParser/CitationParser.o ./PubmedParser/TitleParser.o ./PubmedParser/AbstractParser.o ./PubmedParser/KeywordsParser.o ./PubmedParser/AuthorParser.o ./PubmedParser/PubDateParser.o 

DATAMODEL_DEPS = ./DataModel/CArticle.o ./DataModel/CArticleParsed.o ./DataModel/CDate.o ./DataModel/CName.o ./DataModel/CSection.o ./DataModel/CCitation.o ./DataModel/CReference.o

DB_DEPS = ./DBmanager/DBBasic.o ./DBmanager/DBLowLevelManager.o ./DBmanager/DBManager.o 

XMLGENERATOR_DEPS = ./XMLGenerator/BasicGenerator.o ./XMLGenerator/ArticlesXMLGenerator.o ./XMLGenerator/ExtendedArticlesXMLGenerator.o ./XMLGenerator/CitationsXMLGenerator.o ./XMLGenerator/XMLGenerator.o

IR_DEPS= ./InformationRetrievalSystem/IR.o ./InformationRetrievalSystem/IRTitleBased.o ./InformationRetrievalSystem/IRCitationBased.o  ./InformationRetrievalSystem/IRTester.o

DEPENDENCIES = $(PUGIXML_DEPS) $(DATAMODEL_DEPS) $(PARSER_DEPS) $(DB_DEPS) $(XMLGENERATOR_DEPS) $(IR_DEPS)

#swig flags and constants
PYLIB = /usr/lib
PYINC = /usr/include/python2.7
SWIG = swig


#MAKE DIRECTIVES
all: makeModules DatasetGenerator.o main 

makeModules: 
	cd ./DataModel; make
	cd ./Pugixml; make
	cd ./PubmedParser; make
	cd ./DBmanager; make
	cd ./XMLGenerator; make
	cd ./InformationRetrievalSystem; make

#C++ EXECUTABLE
main: main.o 
	#$(CC) $(CFLAGS) -s main.o  $(PUGIXML_DEPS) $(DATAMODEL_DEPS) $(PARSER_DEPS) $(DB_DEPS) $(INDEXGENERATOR_DEPS) DatasetGenerator.o -o main -L/usr/include/mysql -lmysqlclient
	$(CC) $(CFLAGS) -s main.o  $(DEPENDENCIES) DatasetGenerator.o -o main -L/usr/include/mysql -lmysqlclient

main.o: mainTest.cpp 
	$(CC) $(CFLAGS) $(mysql_config --cflags) -c mainTest.cpp -o main.o $(mysql_config --libs)


DatasetGenerator.o: DatasetGenerator.cpp
	$(CC) $(CFLAGS)  $(mysql_config --cflags) -c DatasetGenerator.cpp -o DatasetGenerator.o $(mysql_config --libs)

#PYTHON WRAPER
_DatasetGenerator.so: DatasetGenerator_wrap.o DatasetGenerator.o
	$(CC) $(CFLAGS)  -shared DatasetGenerator_wrap.o DatasetGenerator.o $(DEPENDENCIES) -L/usr/include/mysql -lmysqlclient -L$(PYLIB) -lpython2.7 -o $@

DatasetGenerator_wrap.o: DatasetGenerator_wrap.cxx DatasetGenerator.h
	$(CC) $(CFLAGS)  $(mysql_config --cflags) DatasetGenerator_wrap.cxx -c -g $(mysql_config --libs) -I$(PYINC) 

DatasetGenerator_wrap.cxx: DatasetGenerator.i
	$(SWIG) -c++ -python DatasetGenerator.i

DatasetGenerator.py: DatasetGenerator.i
	$(SWIG) -c++ -python DatasetGenerator.i

clean:
	rm -f *.o
	rm -f *~
	cd ./DataModel; make clean
	cd ./Pugixml; make clean
	cd ./PubmedParser; make clean
	cd ./DBmanager; make clean
	cd ./XMLGenerator; make clean
	cd ./InformationRetrievalSystem; make clean



