CC=gcc
CFLAGS=-Iheader -Wall -Werror -g -ggdb
LDFLAGS=-lpthread

CXX=g++
CXXFLAGS=-Iheader -Wall -Werror -g -ggdb
LDXXFLAGS=-lpthread

RM=rm
RMFLAGS=-rf

TARGET=build/main.out
OBJS_S=
OBJS_C=obj/lexico.o obj/sintatico.o obj/main.o

all: $(TARGET)

clean:
	$(RM) $(RMFLAGS) $(OBJS_S) $(OBJS_C) $(TARGET)

run:	$(TARGET)
	./build/main.out

build/main.out:	$(OBJS_C)
		$(CXX) -o $@ $(OBJS_C) $(LDXXFLAGS)

obj/lexico.o: header/parser/lexico.hpp src/lexico.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/lexico.cpp

obj/sintatico.o: obj/lexico.o header/parser/sintatico.hpp src/sintatico.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/sintatico.cpp

obj/main.o: obj/sintatico.o src/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/main.cpp
