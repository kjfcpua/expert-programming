finddir=$(shell \
	DIR=$$(echo $1); \
	if [ -d $${DIR} ]; then echo $${DIR}; \
	else echo ""; \
	fi;)

findpdir=$(shell \
	DIR=$$(echo $1); LEVEL=$$(echo $2); D1=$$(echo $3); D2=$$(echo $4); D3=$$(echo $5); \
	for i in $$(seq 32); do \
		NAME=$$(basename $${DIR}); \
		if   [ "$${NAME}" == "$${D1}" ]; then echo $${DIR}; break; \
		elif [ "$${NAME}" == "$${D2}" ]; then echo $${DIR}; break; \
		elif [ "$${NAME}" == "$${D3}" ]; then echo $${DIR}; break; \
		elif [ $${LEVEL} -ge 1 ] && [ -n "$${D1}" ] && [ -d $${DIR}/$${D1} ]; then echo $${DIR}/$${D1}; break; \
		elif [ $${LEVEL} -ge 1 ] && [ -n "$${D2}" ] && [ -d $${DIR}/$${D2} ]; then echo $${DIR}/$${D2}; break; \
		elif [ $${LEVEL} -ge 1 ] && [ -n "$${D3}" ] && [ -d $${DIR}/$${D3} ]; then echo $${DIR}/$${D3}; break; \
		else DIR=$$(dirname $${DIR}); \
		fi; \
	done)

CC=gcc-7
CXX=g++-7

DEBUG=_DEBUG_MORE
OPTIMZE=false
VERBOSE=true
NOW=$(shell date '+%y%m%d%H')

HOME=$(shell pwd | sed "s/trunk\/.*/trunk/")
OPENSSL_HOME=$(call finddir, "/usr/local/Cellar/openssl/1.0.2h_1")

CFLAGS=-Wall -Werror -fno-strict-aliasing -Wno-array-bounds -D_GNU_SOURCE -I/usr/local/include
CXXFLAGS=-std=c++17 -Wall -Werror -fno-strict-aliasing -Wno-array-bounds -Wno-error=unused-local-typedefs -D_GNU_SOURCE -I/usr/local/include
ifneq ($(shell uname | grep "Darwin"),)
	LDFLAGS=-L/usr/local/lib -lcrc32c -lglog -lgflags -lm -lc
else
	LDFLAGS=-L/usr/local/lib -Wl,-static -Wl,-call_shared -lglog -lgflags -lcrypt -lm -lc
endif

ifneq ($(VERBOSE), true)
	CC:=@$(CC)
endif

ifdef DEBUG
	CFLAGS+=-D$(DEBUG)
	CXXFLAGS+=-D$(DEBUG)
endif

ifeq ($(OPTIMZE), true)
	CFLAGS+=-O2 -fvisibility=hidden
	CXXFLAGS+=-O2 -fvisibility=hidden
	LDFLAGS+=-O2
else
	CFLAGS+=-ggdb
	CXXFLAGS+=-ggdb
endif

ifdef ORACLE_HOME
	CFLAGS+=-D_ORACLE -I$(ORACLE_HOME)/rdbms/public
	CXXFLAGS+=-D_ORACLE -I$(ORACLE_HOME)/rdbms/public
	LDFLAGS+=-L$(ORACLE_HOME)/lib \
		-Wl,-rpath=$(ORACLE_HOME)/lib:/home/app/oracle:/home/oracle/lib:/home/umc/oracle/db_1/lib \
		-Wl,-static -locci11 \
		-Wl,-call_shared -lnsl -lclntsh -lnnz11
endif

ifneq ($(OPENSSL_HOME),)
	CFLAGS+=-D_OPENSSL -I$(OPENSSL_HOME)/include
	CXXFLAGS+=-D_OPENSSL -I$(OPENSSL_HOME)/include
	LDFLAGS+=-L$(OPENSSL_HOME)/lib -lcrypto -lssl
endif

ifneq ($(shell uname | grep "Cygwin"),)
	CFLAGS+=-D_KERNEL
	LDFLAGS+=-Wl,--stack,8388608
endif

SRCS=$(wildcard *.c *.cc *.cpp)
BINS=$(patsubst %.c,%,$(patsubst %.cc,%,$(patsubst %.cpp,%,$(SRCS))))
OBJS=$(patsubst %.c,%.o,$(patsubst %.cc,%.o,$(patsubst %.cpp,%.o,$(SRCS))))

%.o:%.c
	$(CC) -o $@ $^ -c $(CFLAGS)

%.o:%.cc
	$(CXX) -o $@ $^ -c $(CXXFLAGS)

%.o:%.cpp
	$(CXX) -o $@ $^ -c $(CXXFLAGS)

%:%.o
	$(CXX) -o $@ $^ $(LDFLAGS)

all:$(OBJS)
	echo $(BINS)

clean:
	rm -f $(OBJS) $(BINS)

tags:
	@if [ -f $(HOME)/tags ]; then rm $(HOME)/tags; fi
ifeq ($(shell uname | grep "Darwin"),)
	ctags -f $(HOME)/tags -aR --c-types=+px $(HOME)/*
else
	ctags -f $(HOME)/tags -aw $(HOME)/*
endif

.PHONY:all release install clean tags

