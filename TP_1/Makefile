CC=gcc
CFLAGS=-Iheader -Wall -Werror -g -ggdb
LDFLAGS=-lpthread

CXX=g++
CXXFLAGS=-Iheader -Wall -Werror -g -ggdb
LDXXFLAGS=-lpthread

RM=rm
RMFLAGS=-rf

TARGET=build/server.out build/client.out
OBJS_S=obj/exec.o obj/message.o obj/server.o
OBJS_C=

all: $(TARGET)

clean:
	$(RM) $(RMFLAGS) $(OBJS_S) $(OBJS_C) $(TARGET)

run:	$(TARGET)
	./build/server.out

build/server.out:	$(OBJS_S)
		$(CC) -o $@ -c src/server.c $(LDFLAGS)
		#$(CXX) -o $@ $(OBJS_S) $(LDXXFLAGS)

build/client.out:	$(OBJS_C)
		$(CC) -o $@ -c src/client.c $(LDFLAGS)
		#$(CXX) -o $@ $(OBJS_C) $(LDXXFLAGS)

obj/exec.o: header/exec.hpp src/exec.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/exec.cpp

obj/message.o: header/message.hpp src/message.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/message.cpp

obj/server.o: header/server.hpp src/server.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/server.cpp

obj/client.o: header/client.hpp src/client.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ src/client.cpp
