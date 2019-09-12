all:
	mpicc queen.c -o queenpb

cleanq:
	rm -rf *.o queenpb
