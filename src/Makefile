

LD = ld
CC = g++
CFLAGS = -I../include
LFLAGS = -pthread -g
TARGET = cppip
export LD CC CFLAGS

NET_STACK_OBJS =	net/net_obj.o	\
					level2/level2_obj.o \
			main.o

all:$(TARGET)
$(TARGET):$(NET_STACK_OBJS)
	$(CC) $(LFLAGS) $^ -o $@

net/net_obj.o:net/*.cpp
	@make -C net/
	
level2/level2_obj.o:level2/*.cpp
	@make -C level2/

main.o:main.cpp
	$(CC) -I./include -c -o $@ $<

clean:
	find . -name "*.o" | xargs rm -f
	rm -rf $(TARGET)