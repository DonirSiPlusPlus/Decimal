CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 #-pedantic #-fsanitize=address
LIBS=-lcheck -lm
LEAK_CHECK=leaks --atExit -- ./s21_test

OS=$(shell uname)

ifeq ($(OS), Darwin) # MAC
OPEN= open ./report/index.html
LIBS+= -L /usr/local/lib
endif

ifeq ($(OS), Linux) # LINUX
OPEN=
LIBS+= -lsubunit -lpthread -lrt #-D_GNU_SOURCE
LEAK_CHECK=valgrind ./s21_test
endif

all: s21_decimal.a test gcov_report

clean:
	rm -Rf *.o *.a *.gcno *.gcda *.info report s21_test s21_decimal s21_test_report valgrind_output
	rm -rf .clang-format

test: test_build
	./s21_test

clang:
  cp ../materials/linters/.clang-format .
	clang-format -i *.c *.h
	clang-format -n *.c *.h

s21_decimal.a: clean s21_additional.c s21_arithmetic.c s21_comparison.c s21_converter.c s21_decimal.h s21_math_funcs.c
	$(CC) -c s21_additional.c s21_arithmetic.c s21_comparison.c s21_converter.c s21_math_funcs.c -lm
	ar rc s21_decimal.a s21_additional.o s21_arithmetic.o s21_comparison.o s21_converter.o s21_math_funcs.o
	ranlib s21_decimal.a

gcov_report: clean s21_additional.c s21_arithmetic.c s21_comparison.c s21_converter.c s21_decimal.h s21_math_funcs.c s21_tests_decimal.c
	$(CC) -fprofile-arcs -ftest-coverage --coverage *.c -o s21_test_report $(LIBS)
	./s21_test_report
	lcov -t s21_test_report -o test.info -c -d .
	genhtml -o report test.info
	$(OPEN)

leaks: test_build
	$(LEAK_CHECK)

check:
	cppcheck *.c *.h

test_build: test.o s21_decimal.a s21_tests_decimal.o
	$(CC) s21_tests_decimal.o -L. s21_decimal.a -o s21_test $(LIBS) 

test.o: s21_tests_decimal.c
	$(CC) -c s21_tests_decimal.c $(LIBS)

s21_decimal.o: s21_arithmetic.c s21_comparison.c s21_converter.c s21_math_funcs.c s21_additional.c
	gcc $(CFLAGS) -c *.c

s21_arithmetic.o: s21_arithmetic.c
	gcc -c s21_arithmetic.c

s21_comparison.o: s21_comparison.c
	gcc -c s21_comparison.c

s21_converter.o: s21_converter.c
	gcc -c s21_converter.c

s21_math_funcs.o: s21_math_funcs.c
	gcc -c s21_math_funcs.c

s21_additional.o: s21_additional.c
	gcc -c s21_additional.c

