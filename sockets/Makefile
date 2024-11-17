CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g
SRC = todo_api_4.c tasks.c todo_api_checks.c
SRC_5 = todo_api_5.c tasks.c todo_api_checks.c
HEADERS = todos.h

todo_api_0: todo_api_0.c
	$(CC) $(CFLAGS) -o todo_api_0 todo_api_0.c

todo_api_1: todo_api_1.c
	$(CC) $(CFLAGS) -o todo_api_1 todo_api_1.c

todo_api_2: todo_api_2.c
	$(CC) $(CFLAGS) -o todo_api_2 todo_api_2.c

todo_api_3: todo_api_3.c
	$(CC) $(CFLAGS) -o todo_api_3 todo_api_3.c

todo_api_4: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o todo_api_4

todo_api_5: $(SRC_5) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC_5) -o todo_api_5

clean:
	rm -f todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5

.PHONY: clean
