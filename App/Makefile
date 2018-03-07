SUBDIRS = libusb client

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean: 
	$(MAKE) cleanall -C libusb
	$(MAKE) cleanall -C client

.PHONY: $(SUBDIRS)
