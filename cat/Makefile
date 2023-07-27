CC=gcc
# пока пишем код
# CFLAGS=-Wall -Werror -Wextra -std=c11 -fsanitize=address -pedantic
# для valgrind и вертера
CFLAGS=-Wall -Werror -Wextra -std=c11 -pedantic

.PHONY: clean all rebuild style s21_cat

all: s21_cat

s21_cat: s21_cat.o
	@$(CC) $(CFLAGS) $^ -o ./s21_cat

s21_cat.o: s21_cat.c s21_cat.h
	@$(CC) $(CFLAGS) -c s21_cat.c

#

clean:
	@rm -rf *.o ./s21_cat

rebuild:
	@$(MAKE) clean
	@$(MAKE) all

style:
	cp ../../materials/linters/.clang-format ./
	clang-format --Werror -i *.c *.h
	rm ./.clang-format