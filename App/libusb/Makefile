CC = gcc
SRC = main.c
CFLAGS = -Wextra -Wall
SUBDIRS = Libusb Communication
LOCAL_LIBS  = -L Communication/  -L Libusb/
LOCAL_LIBS += -lSocketServ  -lXbeeController
LIBS = -lusb-1.0
CFLAGS     += -I Communication/  -I Libusb/
EXEC = app

all: $(EXEC)  

$(SUBDIRS):
	$(MAKE) -c $@

debug:
	$(MAKE) -C Libusb debug
	$(MAKE) -C Communication debug
	$(CC) $(SRC) -o $(EXEC) $(LOCAL_LIBS) $(CFLAGS) $(LIBS) -DDEBUG

clean: 
	rm -f *.o $(EXEC)
	$(MAKE) -C Libusb clean
	$(MAKE) -C Communication clean	

$(EXEC):
	$(MAKE) -C Libusb
	$(MAKE) -C Communication
	$(CC) $(SRC) -o $(EXEC) $(LOCAL_LIBS) $(CFLAGS) $(LIBS)
