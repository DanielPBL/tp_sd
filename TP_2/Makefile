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
OBJS_C=obj/lexico.o obj/sintatico.o obj/parser.o obj/main.o \
	   obj/message.o obj/msgfct.o

INC_CMDS=header/parser/comando.hpp \
		 header/parser/quitcmd.hpp \
		 header/parser/helpcmd.hpp
OBJS_CMDS=obj/comando.o obj/helpcmd.o obj/quitcmd.o

all: $(TARGET)

clean:
	$(RM) $(RMFLAGS) $(OBJS_S) $(OBJS_C) $(TARGET) $(OBJS_CMDS)

run:	$(TARGET)
	./build/main.out

build/main.out:	$(OBJS_C) $(OBJS_CMDS)
		$(CXX) -o $@ $^ $(LDXXFLAGS)

obj/lexico.o: header/parser/lexico.hpp src/lexico.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/lexico.cpp

obj/sintatico.o: header/parser/lexico.hpp $(INC_CMDS)\
	header/parser/sintatico.hpp src/sintatico.cpp
		$(CXX) $(CXXFLAGS) -c -o $@ src/sintatico.cpp

obj/parser.o: header/parser/sintatico.hpp header/parser/parser.hpp src/parser.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/parser.cpp

obj/comando.o: header/parser/comando.hpp src/comando.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/comando.cpp

obj/message.o: header/message.hpp src/message.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/message.cpp

obj/msgfct.o: header/message.hpp header/msgfct.hpp src/msgfct.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/msgfct.cpp

obj/helpcmd.o: header/parser/comando.hpp header/parser/helpcmd.hpp src/helpcmd.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/helpcmd.cpp

obj/quitcmd.o: header/parser/comando.hpp header/parser/quitcmd.hpp src/quitcmd.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/quitcmd.cpp

obj/main.o: header/parser/parser.hpp src/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/main.cpp
