CC=gcc
CFLAGS=-g -Wall -Wextra

default: 
	@$(CC) $(CFLAGS) -o lab3a lab3a.c

dist:
	@@tar -czf lab3a-104732121.tar.gz README Makefile lab3a.c
