CC=gcc
CFLAGS=-Iheader -Wall -Werror -g -ggdb
LDFLAGS=-lpthread

CXX=g++
CXXFLAGS=-Iheader -Wall -Werror -g -ggdb
LDXXFLAGS=-lpthread

RM=rm
RMFLAGS=-rf

TARGET=build/peer.out
OBJS=obj/lexico.o obj/sintatico.o obj/parser.o obj/main.o \
	 obj/message.o obj/msgfct.o obj/peer.o

INC_CMDS=header/parser/comando.hpp \
		 header/parser/helpcmd.hpp \
		 header/parser/findcmd.hpp \
		 header/parser/storecmd.hpp \
		 header/parser/entercmd.hpp
OBJS_CMDS=obj/comando.o obj/helpcmd.o obj/findcmd.o obj/storecmd.o obj/entercmd.o

all: $(TARGET)

clean:
	$(RM) $(RMFLAGS) $(OBJS) $(TARGET) $(OBJS_CMDS)

run:	$(TARGET)
	./build/peer.out

build/peer.out:	$(OBJS) $(OBJS_CMDS)
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

obj/findcmd.o: header/parser/comando.hpp header/parser/findcmd.hpp src/findcmd.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/findcmd.cpp

obj/storecmd.o: header/parser/comando.hpp header/parser/storecmd.hpp src/storecmd.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/storecmd.cpp

obj/entercmd.o: header/parser/comando.hpp header/parser/entercmd.hpp src/entercmd.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/entercmd.cpp

obj/peer.o: header/peer.hpp src/peer.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/peer.cpp

obj/main.o: header/peer.hpp src/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/main.cpp
