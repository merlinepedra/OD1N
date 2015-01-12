CC=gcc
#CFLAGS=-W -Wall -Wextra -O2 -fstack-protector-all
#DFLAGS=-D_FORTIFY_SOURCE=2
#LDFLAGS=-Wl,-z,relro,-z,now -lcurl

LDFLAGS=-lcurl


0d1n: 0d1n.c 
	$(CC) $(CFLAGS) $(DFLAGS) -c *.c
	$(CC) -o 0d1n *.o $(LDFLAGS)

clean:
	rm -f *.o 0d1n
