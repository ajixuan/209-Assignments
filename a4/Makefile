CC = gcc
CFLAGS = -Wall -Werror -g

PORT=56495
CFLAGS= -DPORT=\$(PORT) -g -Wall

buxserver: buxserver.o lists.o lists.h buxfer.o
	$(CC) $(CFLAGS) -o buxserver buxfer.o lists.o buxserver.o

buxfer.o: buxfer.c lists.h
	$(CC) $(CFLAGS) -c buxfer.c

lists.o: lists.c lists.h
	$(CC) $(CFLAGS) -c lists.c

buxserver.o: buxserver.c lists.h
	$(CC) $(CFLAGS) -c buxserver.c

clean: 
	rm buxfer *.o
