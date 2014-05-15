# Makefile for embedded Python use demo.
# (This version tailored for my Red Hat Linux 6.1 setup;
# edit lines marked with XXX.)

# XXX The compiler you are using
CC=	 	gcc

# Compiler flags
OPT=-O3
INCLUDES=	
CFLAGS=		$(OPT)
CPPFLAGS=	$(INCLUDES)

LIBS=		-lrt
LDFLAGS=	

all:		cpubench

cpubench:	cpubench.o
		$(CC) $(LDFLAGS) cpubench.o $(LIBS) -o cpubench

cpubench.o:
		$(CC) -c $(CFLAGS) cpubench.c

clean:
		-rm -f *.o core cpubench
