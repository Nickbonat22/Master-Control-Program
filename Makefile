CC = gcc
CFLAGS = -g -Wall

default: part1 part2 part3 part4

part1: part1.c
	$(CC) $(CFLAGS) $^ -o $@

part2: part2.c
	$(CC) $(CFLAGS) $^ -o $@

part3: part3.c
	$(CC) $(CFLAGS) $^ -o $@

part4: part4.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f part1 part2 part3 part4