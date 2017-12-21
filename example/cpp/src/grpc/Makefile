CC=gcc
CXX=g++

DEBUG=_DEBUG_MORE
OPTIMZE=false
VERBOSE=true
NOW=$(shell date '+%y%m%d%H')

HOME=$(shell pwd | sed "s/trunk\/.*/trunk/")

CXXFLAGS=-std=c++11 -Wall -Werror -fno-strict-aliasing \
         -Wno-array-bounds -Wno-error=unused-local-typedefs \
         `pkg-config --cflags protobuf grpc` \
         -D_GNU_SOURCE -I/usr/local/include
ifneq ($(shell uname | grep "Darwin"),)
	LDFLAGS=-L/usr/local/lib \
	        `pkg-config --libs protobuf grpc++ grpc` \
	        -lgrpc++_reflection \
	        -ldl
else
	LDFLAGS=-L/usr/local/lib -Wl,-static -Wl,-call_shared -lglog -lgflags -lm -lc
endif

ifneq ($(VERBOSE), true)
	CC:=@$(CC)
endif

ifdef DEBUG
	CXXFLAGS+=-D$(DEBUG)
endif

ifeq ($(OPTIMZE), true)
	CXXFLAGS+=-O2 -fvisibility=hidden
	LDFLAGS+=-O2
else
	CXXFLAGS+=-ggdb
endif

SRCS=$(wildcard *.pb.c *.pb.cc *.pb.cpp)
OBJS=$(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SRCS))))
BINS=arith_sync_server arith_async_server

%.o:%.c
	$(CC) -o $@ $^ -c $(CFLAGS)

%.o:%.cc
	$(CXX) -o $@ $^ -c $(CXXFLAGS)

%.o:%.cpp
	$(CXX) -o $@ $^ -c $(CXXFLAGS)

%:%.o
	$(CXX) -o $@ $^ $(LDFLAGS)

all:$(BINS)

arith_sync_server:arith_sync_server.o $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
	
arith_async_server:arith_async_server.o $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(BINS)
	rm -f *.o
	rm -rf *.dSYM

tags:
	@if [ -f $(HOME)/tags ]; then rm $(HOME)/tags; fi
ifeq ($(shell uname | grep "Darwin"),)
	ctags -f $(HOME)/tags -aR --c-types=+px $(HOME)/*
else
	ctags -f $(HOME)/tags -aw $(HOME)/*
endif

.PHONY:all release install clean tags

