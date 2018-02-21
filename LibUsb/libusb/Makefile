CC = gcc
CFLAGS = -W -Wall -pedantic
LDFLAGS = -lusb-1.0 
EXEC = libusb
SRC = $(wildcard *.c) #définit la liste des .o du dossier

all : $(EXEC)

libusb: $(SRC)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) 

debug: $(SRC)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) -g -DDEBUG

clean:
	rm -rf *.o

cleanall: clean
	rm -rf $(EXEC)
	rm -rf debug
