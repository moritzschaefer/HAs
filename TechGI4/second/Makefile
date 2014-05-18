all:
	gcc -o udpc udpclient6.c
test: test_hash.c hash.h
	gcc -g test_hash.c -o test && ./test
server: udpserver6.c
	gcc -o udpserver6 udpserver6.c
