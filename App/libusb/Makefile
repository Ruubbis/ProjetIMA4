CC = gcc
CFLAGS = -Wextra -Wall -pedantic
LDFLAGS = -lusb-1.0 
EXEC = socketsServer
SRC = $(wildcard *.c) #définit la liste des .o du dossier
OBJ = $(SRC:.c=.o)	
OBJ_DEBUG = $(SRC:.c=_debug.o)

all : $(EXEC)
debug : $(EXEC)_debug

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

$(EXEC)_debug: $(OBJ_DEBUG)
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

%.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS)


%_debug.o : %.c
	$(CC) -o $@ -c $< $(CFLAGS) -DDEBUG

clean:
	rm -rf *.o

cleanall: clean
	rm -rf $(EXEC) $(EXEC)_debug
