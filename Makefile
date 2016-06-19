CC := gcc
FLAGS := -Wall -Wextra

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

lib: str_escape.o

test: str_escape.c
	$(CC) $(FLAGS) -DRUN_TESTS $< -o test && ./test
clean:
	rm -rf test *.o
