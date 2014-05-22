CC=icc

# Compiler flags
OPT=-Ofast
INCLUDES=	
CFLAGS=-Wall $(OPT)
CPPFLAGS=	$(INCLUDES)

LIBS=		-lrt
LDFLAGS=	

all:		cpubench

cpubench:	cpubench.o dummy.o
		$(CC) $(LDFLAGS) dummy.o cpubench.o $(LIBS) -o cpubench

cpubench.o:
		$(CC) -c $(CFLAGS) cpubench.c
dummy.o:
		$(CC) -c $(CFLAGS) dummy.c

clean:
		-rm -f *.o core cpubench
