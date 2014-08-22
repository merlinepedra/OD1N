CC=gcc
CFLAGS=-Wall -Wextra -lcurl -Ofast -fstack-protector-all -march=native -D_FORTIFY_SOURCE=2 -Wl,-z,relro,-z,now 
BINDIR=/usr/bin

0d1n: 0d1n.c 
	$(CC) $(CFLAGS) -c *.c 
	$(CC) $(CFLAGS) -o 0d1n *.o 
	rm *.o

clean:
	rm 0d1n
