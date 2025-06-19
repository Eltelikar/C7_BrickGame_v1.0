CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11 
SANITIZE=-fsanitize=address
TST_LIBS =-lcheck

DIRS = brick_game/tetris gui/cli
CLEAN_EXTS = gcno gcda gcov css profraw o save

ifeq ($(shell uname), Linux)
	TST_LIBS += -lpthread -lrt 
endif

LIB_NAME = s21_tetris
BACK=$(wildcard brick_game/tetris/*.c)
OBJ_BACK=$(BACK:.c=.o)
TEST=$(wildcard tests/tests.c)

all: install

%.o: %.c
	@$(CC) $(FLAGS) -c $< -o $@

mkbuild:
	@rm -rf build/
	@mkdir build

uninstall :
	@rm -rf build/
	@echo "\033[31mGame uninstalled \033[0m"

install : mkbuild main
	@echo "\033[32mGame installed \033[0m"

back_lib : $(OBJ_BACK) 
	@ar rc build/back.a $^
	@ranlib build/back.a 
	@rm brick_game/tetris/*.o

front_lib:
	@$(CC) $(FLAGS) -c gui/cli/frontend.c -o gui/cli/frontend.o
	@ar rc build/front.a gui/cli/frontend.o
	@ranlib build/front.a 
	@rm gui/cli/*.o

main: back_lib front_lib
	@$(CC) $(FLAGS) tetris.c -o build/TETRIS build/back.a build/front.a -lncurses

clean:
	for dir in $(DIRS); do\
		for ext in $(CLEAN_EXTS); do\
			find $$dir -name "*.$$ext" -type f -delete;\
		done;\
	done 
	rm -f *.o *.a test *.gcno *.gcda *.gcov *.css *.html *.profraw *.save *.gz
	rm -rf doxygen

dvi:
	@doxygen .Doxyfile
	@open doxygen/html/index.html

dist: install
	@rm -rf dist.tar.gz
	@tar -cvzf dist.tar.gz build/TETRIS

add_coverage_flag:
	$(eval FLAGS += --coverage)

check-clang:
	clang-format -n -style=Google tetris.c brick_game/tetris/* gui/cli/*

check-leak: test
	valgrind --leak-check=full ./test

test : clean mkbuild back_lib front_lib
	$(CC) $(FLAGS) $(TEST) build/back.a  build/front.a -lcheck -lsubunit -lm -lpthread -lrt -lncurses -o test

gcov_report: clean add_coverage_flag mkbuild back_lib front_lib test
	./test
	gcov -b -l -p -c brick_game/tetris/backend.c brick_game/tetris/utilsFunctions.c

	gcovr -r . --html --html-details -o gcov_report.html --exclude 'tetris.c|tests/*|gui/cli/*'
