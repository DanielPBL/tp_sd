CC=gcc
CFLAGS=-Iheader -Wall -Werror -g -ggdb
LDFLAGS=-lpthread

CXX=g++
CXXFLAGS=-Iheader -Wall -Werror -g -ggdb
LDXXFLAGS=-lpthread

RM=rm
RMFLAGS=-rf

TARGET=build/server.out build/client.out
OBJS_S=obj/exec.o obj/message.o obj/server.o obj/s_main.o
OBJS_C=obj/message.o obj/client.o obj/c_main.o obj/server.o

all: $(TARGET)

clean:
	$(RM) $(RMFLAGS) $(OBJS_S) $(OBJS_C) $(TARGET)

run:	$(TARGET)
	./build/server.out

build/server.out:	$(OBJS_S)
		$(CXX) -o $@ $(OBJS_S) $(LDXXFLAGS)

build/client.out:	$(OBJS_C)
		$(CXX) -o $@ $(OBJS_C) $(LDXXFLAGS)

obj/exec.o: header/exec.hpp src/exec.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/exec.cpp

obj/message.o: header/message.hpp src/message.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/message.cpp

obj/server.o: header/server.hpp src/server.cpp obj/message.o obj/exec.o
	$(CXX) $(CXXFLAGS) -c -o $@ src/server.cpp

obj/client.o: header/client.hpp src/client.cpp obj/message.o
	$(CXX) $(CXXFLAGS) -c -o $@ src/client.cpp

obj/s_main.o: obj/server.o src/s_main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/s_main.cpp

obj/c_main.o: obj/client.o src/c_main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/c_main.cpp
