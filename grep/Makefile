CC=gcc
# пока пишем код
# CFLAGS=-Wall -Werror -Wextra -std=c11 -fsanitize=address -pedantic
# для valgrind и вертера
CFLAGS=-Wall -Werror -Wextra -std=c11 -pedantic

.PHONY: clean all rebuild style s21_grep

all: s21_grep

s21_grep: s21_grep.o
	@$(CC) $(CFLAGS) $^ -o ./s21_grep

s21_grep.o: s21_grep.c s21_grep.h
	@$(CC) $(CFLAGS) -c s21_grep.c

#

clean:
	@rm -rf *.o ./s21_grep

rebuild:
	@$(MAKE) clean
	@$(MAKE) all

style:
	cp ../../materials/linters/.clang-format ./
	clang-format --Werror -i *.c *.h
	rm ./.clang-format