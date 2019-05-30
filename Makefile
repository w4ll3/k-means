CC=g++
CFLAGS=-O3 -lpthread

all: par seq

par:
	$(CC) k-means.par.cpp $(CFLAGS) -o k-means-par

seq:
	$(CC) k-means.seq.cpp $(CFLAGS) -o k-means-seq

.PHONY:
	clear

clear:
	rm -rf k-means-* results/my*