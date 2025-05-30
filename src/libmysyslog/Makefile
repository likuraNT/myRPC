CC = gcc
CFLAGS = -Wall -Wextra -fPIC -I./include
TARGET = libmysyslog.so

all: $(TARGET)

$(TARGET): libmysyslog.o
	$(CC) -shared -o $(TARGET) libmysyslog.o

libmysyslog.o: libmysyslog.c
	$(CC) $(CFLAGS) -c libmysyslog.c

clean:
	rm -f $(TARGET) *.o

deb: clean all
	mkdir -p deb_pkg/usr/lib
	mkdir -p deb_pkg/usr/include
	mkdir -p deb_pkg/DEBIAN	
	touch deb_pkg/DEBIAN/control
	install -m 644 $(TARGET) deb_pkg/usr/lib/
	install -m 644 ../../include/libmysyslog/libmysyslog.h deb_pkg/usr/include/
	echo "Package: libmysyslog" > deb_pkg/DEBIAN/control
	echo "Version: 1.0-1" >> deb_pkg/DEBIAN/control
	echo "Section: libs" >> deb_pkg/DEBIAN/control
	echo "Priority: optional" >> deb_pkg/DEBIAN/control
	echo "Architecture: amd64" >> deb_pkg/DEBIAN/control
	echo "Maintainer: <likuraNT>" >> deb_pkg/DEBIAN/control
	echo "Description: My custom syslog library" >> deb_pkg/DEBIAN/control
	chmod 755 -R deb_pkg/
	fakeroot dpkg-deb --build deb_pkg ../../deb/libmysyslog_1.0-1_amd64.deb
	rm -rf deb_pkg

.PHONY: all clean deb
