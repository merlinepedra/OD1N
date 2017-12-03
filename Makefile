export MALLOC_MMAP_THRESHOLD_=1
export MALLOC_CHECK_=1
export MALLOC_PERTURB_=1
CC=gcc
CFLAGS=-W -Wall -Wextra -O2 -fstack-protector-all
DFLAGS=-D_FORTIFY_SOURCE=2
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS=-Wl,-lcurl
else
	LDFLAGS=-Wl,-z,relro,-z,now -lcurl
endif

#LDFLAGS=-lcurl


0d1n: 0d1n.c 
	$(CC) $(CFLAGS) $(DFLAGS) -c *.c
	$(CC) -o 0d1n *.o $(LDFLAGS)

clean:
	rm -f *.o 0d1n

PREFIX=/usr/local
install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 0d1n $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/0d1n
