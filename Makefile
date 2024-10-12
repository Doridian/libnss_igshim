%.o: %.c
	$(CC) -c -fPIC -o $@ $^

libnss_igshim.so: libnss_igshim.c
	$(CC) -Wall -Werror -O2 -flto -shared -fPIC -o $@ $^

install: libnss_igshim.so
	install -m 755 -d $(DESTDIR)/usr/lib
	set
	uname -a
	install -m 755 libnss_igshim.so $(DESTDIR)/usr/lib/libnss_igshim.so.2

all: libnss_igshim.so

clean:
	rm -f *.so *.o
