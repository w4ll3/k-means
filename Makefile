CC=g++ -I/usr/include
CFLAGS=-O3 -lpthread -lboost_mpi -lboost_serialization -lboost_system -lboost_filesystem -lboost_graph_parallel -lboost_iostreams

all: par seq

par:
	$(CC) k-means.par.cpp $(CFLAGS) -o k-means-par

seq:
	$(CC) k-means.seq.cpp $(CFLAGS) -o k-means-seq

.PHONY:
	clear

clear:
	rm -rf k-means-* results/my*
