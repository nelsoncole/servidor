all:
	gcc server.c get.c itoa.c -o server
	./server
