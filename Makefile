SUBDIRS := src/libmysyslog src/myRPC-server src/myRPC-client

.PHONY: all clean deb

all:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir || exit 1; \
	done

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

deb: all
	@mkdir -p deb
	$(MAKE) -C src/libmysyslog deb
	$(MAKE) -C src/myRPC-server deb
	$(MAKE) -C src/myRPC-client deb