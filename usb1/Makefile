# Makefile for ft245rl resource manager
#


PROG = read_ft245
USE = read_ft245.c 
SRC = read_ft245.c 

DEBUG = -g


CFLAGS  =  -w9 $(DEFS) $(DEBUG) -I ../include
LDFLAGS =  -M $(DEBUG) 

all : $(PROG)

$(PROG) : $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)
	usemsg $@ $(USE)

clean :
	rm -f $(PROG) *.o *.err *.map

install : $(PROG)
	cp -v $(PROG) $(CFGF) ../../bin/	
	wstrip ../../bin/$(PROG)
