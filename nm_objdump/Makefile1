CC = gcc

CFLAGS = -Wall -Wextra -O2

SRC = hnm.c
EXEC = hnm

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

clean:
	rm -f $(EXEC)

.PHONY: clean
