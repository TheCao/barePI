#
# Makefile
#

USPIHOME   = ..

OBJS	= main.o myfunc.o

LIBS	= ../lib/libuspi.a \
	 ../env/lib/libuspienv.a

include Rules.mk

clean:
	rm -f *.o *.a *.elf *.lst *.img *.cir *.map *.hex*~ $(EXTRACLEAN)