OBJS	= arp_parser.o arp_entry.o arp_cache.o
SUBDIR	= level2

all:level2_obj.o
level2_obj.o:$(OBJS)
	$(Q)$(LD) -r -o $@ $^

%.o:%.cpp
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
