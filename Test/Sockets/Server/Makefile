CC = gcc
CFLAGS = -W -Wall -pedantic
LDFLAGS = 
EXEC = socketsServer
SRC = $(wildcard *.c) #définit la liste des .o du dossier

all : $(EXEC)

$(EXEC): $(SRC)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

debug : $(SRC)	
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) -g -DDEBUG

clean:
	rm -rf *.o

cleanall: clean
	rm -rf $(EXEC)
