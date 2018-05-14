CC = javac
SRC = $(wildcard *.java)
CLASS = $(SRC:.java=.class)

all: $(CLASS)

%.class : %.java
	$(CC) $<


clean:
	rm -rf *.class
