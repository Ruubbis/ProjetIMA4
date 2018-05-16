CC = gcc
CFLAGS = -Wextra -Wall 
LDFLAGS =  
OUT = libSocketServ.a
SRC = libSocketServ.c
OBJ = libSocketServ.o


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

