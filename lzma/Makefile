CC = gcc
CFLAGS = -g -Wall -O3

LD = gcc
LDFLAGS =

all: lzmapack lzmaunpack

clean:
	rm -f lzmapack lzmaunpack test.dat

lzmapack:
	gcc -static lzmapack.c -o lzmapack -llzma

lzmaunpack:
	gcc -static lzmaunpack.c -o lzmaunpack -llzma

test:
	cat lzmapack|./lzmapack|./lzmaunpack > test.dat
	diff lzmapack test.dat
