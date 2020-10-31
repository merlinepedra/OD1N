export MALLOC_MMAP_THRESHOLD_=1
export MALLOC_CHECK_=1
export MALLOC_PERTURB_=1
CC=gcc
CFLAGS=-W -Wall -Wextra -Wformat-security
HARDENING= -mmitigate-rop -fstack-protector-all -pie -fPIE -ftrapv
DIR=src/
DIROUT=bin/
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS=-Wl,-lcurl
else
	LDFLAGS=-Wl,-z,relro,-z,now -lcurl
endif



0d1n: $(DIR)0d1n.c 
	$(CC) $(CFLAGS) $(HARDENING) -c $(DIR)*.c
	$(CC) $(HARDENING)  -o $(DIROUT)0d1n *.o  $(LDFLAGS)

clean:
	rm -f *.o 0d1n

PREFIX=/usr/local
install:
	make
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0766 bin/0d1n $(DESTDIR)$(PREFIX)/bin
	mkdir /opt/0d1n
	mkdir /opt/0d1n/view/
	mkdir /opt/0d1n/view/response_sources
	mkdir /opt/0d1n/payloads
	cp -rf templates /opt/0d1n/
	cp -rf tables /opt/0d1n/view/
	cp -rf payloads /opt/0d1n/
	chmod 0766 -R /opt/0d1n
	chown ${USER} -R /opt/0d1n
	chown ${USER} $(DESTDIR)$(PREFIX)/bin/0d1n

uninstall:
	rm $(DESTDIR)$(PREFIX)/bin/0d1n
	rm -rf /opt/0d1n
	rm -f *.o 0d1n
