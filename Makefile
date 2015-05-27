CFLAGS = -g -Wall -Wextra -pedantic -std=c99

test: test.c arena.c

clean:
	rm test

.PHONY: clean