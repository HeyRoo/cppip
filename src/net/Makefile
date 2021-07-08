OBJS	= tap_if.o vir_eth.o pkb.o net_handler.o net_device.o lo_eth.o
SUBDIR	= net

all:net_obj.o
net_obj.o:$(OBJS)
	$(Q)$(LD) -r -o $@ $^

%.o:%.cpp
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
