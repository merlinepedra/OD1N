CC=gcc
CFLAGS=-W -Wall -Wextra -Ofast -fstack-protector-all
DFLAGS=-D_FORTIFY_SOURCE=1
LDFLAGS=-Wl,-z,relro,-z,now -lcurl -g

0d1n: 0d1n.c 
	$(CC) $(CFLAGS) $(DFLAGS) -c *.c
	$(CC) -o 0d1n *.o $(LDFLAGS)

clean:
	rm -f *.o 0d1n
