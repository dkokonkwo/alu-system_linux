CC = gcc

CFLAGS = -Wall -Wextra -O2

SRC = hobjdump.c
EXEC = hobjdump

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

clean:
	rm -f $(EXEC)

.PHONY: clean
