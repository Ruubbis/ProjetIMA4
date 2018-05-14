CC = gcc
CFLAGS = -Wextra -Wall -pedantic
LDFLAGS = -lusb-1.0 
OUT = libXbeeController.a
SRC = libXbeeController.c
OBJ = libXbeeController.o


main: $(OBJ)
	rm -f $(OUT)
	ar cq $(OUT) $+ 

debug:
	$(CC) -o $(OBJ) -c $(SRC) $(CFLAGS) $(LDFLAGS) -DDEBUG
	rm -f $(OUT)
	ar cq $(OUT) $(OBJ)

$(OBJ):
	$(CC) -o $@ -c $(SRC) $(CFLAGS) $(LDFLAGS)


clean:
	rm -rf *.o $(OUT)

