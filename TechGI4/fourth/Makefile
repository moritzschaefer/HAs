all: p2pserver.c hash.h
	gcc -o p2pserver p2pserver.c
run:
	./p2pserver 1234 100 0  127.0.0.1 1235 200 127.0.0.1 1236 & ./p2pserver 1235 0 200 127.0.0.1 1236 100 127.0.0.1 1234 & ./p2pserver 1234 200 100  127.0.0.1 1234 0 127.0.0.1 1235 &

