
# all the source files that need to be compiled and linked
CSRC=lidar-client.c log.c wrapper.c

# the executable program that you want to build
TARGET=lidar-client

# define the following line to enable debugging output 
#CPPFLAGS+=-DENABLE_DEBUG

CFLAGS+=-O 
# CFLAGS+=-Wall -Werror
# CFLAGS+=-Wno-unused-function

LDFLAGS+=-lm


OBJS=$(CSRC:.c=.o)

.PHONY: all clean world

all:: $(TARGET)

$(TARGET):: $(OBJS)
	$(LINK.c) -o $@ $^

clean::
	rm -f *.o $(TARGET)

world:: clean all
