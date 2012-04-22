CC      = gcc
CFLAGS  = -g -Wall -O3 -Ixdelta

LD      = gcc
LDFLAGS =

DD      = dd

all: xdeltapack xdeltaunpack

clean:
	rm -f xdeltapack xdeltaunpack test.dat

xdeltapack: xdeltapack.c
	$(CC) $(CFLAGS) xdeltapack.c -o xdeltapack

xdeltaunpack: xdeltaunpack.c
	$(CC) $(CFLAGS) xdeltaunpack.c -o xdeltaunpack

test: xdeltapack xdeltaunpack
	$(DD) if=/dev/zero    bs=1024 count=14 1> source.dat 2>/dev/null
	$(DD) if=/dev/urandom bs=1024 count=2  1>>source.dat 2>/dev/null
	$(DD) if=/dev/zero    bs=1024 count=2  1>>source.dat 2>/dev/null
	$(DD) if=/dev/zero    bs=1024 count=14 1>>source.dat 2>/dev/null
	
	$(DD) if=/dev/zero    bs=1024 count=14 1> input.dat 2>/dev/null
	$(DD) if=/dev/zero    bs=1024 count=2  1>>input.dat 2>/dev/null
	$(DD) if=/dev/urandom bs=1024 count=2  1>>input.dat 2>/dev/null
	$(DD) if=/dev/zero    bs=1024 count=14 1>>input.dat 2>/dev/null
	
	cat input.dat|./xdeltapack source.dat|./xdeltaunpack source.dat > output.dat
	
	diff input.dat output.dat
