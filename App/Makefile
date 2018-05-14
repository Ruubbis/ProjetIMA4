SUBDIRS = libusb client
SUBDIRS_DEBUG = libusb_debug client
all: $(SUBDIRS)

debug: $(SUBDIRS_DEBUG)

libusb_debug:
	$(MAKE) -C libusb debug

$(SUBDIRS):
	$(MAKE) -C $@

clean: 
	$(MAKE) clean -C libusb
	$(MAKE) clean -C client

.PHONY: $(SUBDIRS)
