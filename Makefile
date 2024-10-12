%.o: %.c
	$(CC) -c -fPIC -o $@ $^

libnss_igshim.so: libnss_igshim.c
	$(CC) -Wall -Werror -O2 -flto -shared -fPIC -o $@ $^

install: libnss_igshim.so
	cp -fv libnss_igshim.so /usr/lib/libnss_igshim.so

all: libnss_igshim.so

clean:
	rm -f *.so *.o
