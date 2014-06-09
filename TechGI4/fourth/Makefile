all: p2pserver.c hash.h p2pclient.c p2pclient2.c p2pserver2.c
	gcc -g -o p2pserver p2pserver.c -lm  && gcc -g -o p2pclient p2pclient.c -lm && gcc -g -o p2pclient2 p2pclient2.c -lm && gcc -g -o p2pserver2 p2pserver2.c -lm & killall p2pserver2
runserver:
	./p2pserver2 1234 64 0 127.0.0.1 1235 128 127.0.0.1 1237 &
	./p2pserver2 1236 192 128 127.0.0.1 1237 0 127.0.0.1 1235 &
	./p2pserver2 1237 128 64 127.0.0.1 1234 192 127.0.0.1 1236 &
	./p2pserver2 1235 0 192 127.0.0.1 1236 64 127.0.0.1 1234 
runclient:
	./p2pclient2 localhost 1236
	


runmainserver:
	./p2pserver2 1235 0 192 127.0.0.1 1236 64 127.0.0.1 1234 
