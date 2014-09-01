CC=gcc
CFLAGS=-Wall -Wextra -lcurl -Ofast -fstack-protector-all -Wl,-z,relro,-z,now -D_FORTIFY_SOURCE=fast
BINDIR=/usr/bin

0d1n: 0d1n.c 
	$(CC) $(CFLAGS) -c *.c 
	$(CC) $(CFLAGS) -o 0d1n *.o 
	rm *.o

clean:
	rm 0d1n
