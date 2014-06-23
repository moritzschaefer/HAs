all: server.c client.c server6a.c server6b.c
	gcc -g -o client client.c &
	gcc -g -o server server.c &
	gcc -g -o server6a server6a.c &
	gcc -g -o server6b server6b.c &
	gcc -g -o server6c server6c.c &
	gcc -g -o client6c client6c.c &
	killall server server6a server6b
runserver:
	 ./server 6789		&
runserver6a:
	./server6a 6789 	&
runserver6b:
	./server6b 6789 	&
runserver6c:
	./server6c 6789 	&

runclient:
	 ./client localhost 6789
runclient6c:
	 ./client6c localhost 6789
