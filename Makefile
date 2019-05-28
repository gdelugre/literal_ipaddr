CXXFLAGS=-Wall -Wextra -Os -std=c++17 -Wno-missing-field-initializers

ifeq ($(WITH_TESTS),1)
CXXFLAGS+=-DWITH_TESTS=1
endif

all: ipaddr
ipaddr: ipaddr.o
ipaddr.o: ipaddr.cc
clean:
	rm -f *.o ipaddr
