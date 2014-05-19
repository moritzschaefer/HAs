client:
	gcc -g -o udpc udpclient6.c
test: test_hash.c hash.h
	gcc -g test_hash.c -o test && ./test
server: udpserver6.c
	gcc -g -o udps udpserver6.c
