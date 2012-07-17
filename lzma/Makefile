CC = gcc
CFLAGS = -g -Wall -O3

all: lzmapack lzmaunpack

clean:
	rm -f lzmapack lzmaunpack test.dat lzmademo.tar.bz2

lzmapack:
	$(CC) $(CFLAGS) lzmapack.c -o lzmapack -llzma

lzmaunpack:
	$(CC) $(CFLAGS) lzmaunpack.c -o lzmaunpack -llzma

test: \
  lzmapack \
  lzmaunpack
	cat lzmapack|./lzmapack|./lzmaunpack > test.dat
	diff lzmapack test.dat

.PHONY: dist
dist:
	tar cjf lzmademo.tar.bz2 \
          lzmapack.c \
          lzmaunpack.c \
          Makefile
