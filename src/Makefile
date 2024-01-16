TESTFLAGS:=
ifeq ($(shell uname -s),Linux)
	TESTFLAGS += -lm -lrt -lsubunit
endif

OPSYS=MAC

TESTFLAGS += -lcheck -lpthread -pthread
GCCFLAGS = -Wall -Werror -Wextra
GCOVFLAGS = -fprofile-arcs -ftest-coverage

all: s21_string.a

s21_string.a: clean
	clang-format -style=Google -i *.c *.h
	gcc $(GCCFLAGS) -g -c s21_*.c
	ar rc s21_string.a *.o
	ranlib s21_string.a
	mkdir binary_files
	mv *.o binary_files

gcov_report: s21_string.a
	gcc $(GCCFLAGS) $(GCOVFLAGS) test_*.c $(TESTFLAGS) s21_*.c -o gcovreport
	./gcovreport
	lcov -t "gcovreport" -o gcovreport.info -c -d . 
	genhtml -o report gcovreport.info 
	open report/src/index.html
	
test: s21_string.a
	gcc $(GCCFLAGS) $(TESTFLAGS) *.c s21_string.a -o string_test
	./string_test

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info binary_files test string_test report gcovreport