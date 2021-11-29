CC=gcc
CFLAGS=-Wall -g
PROGRAM_NAME=videostego
PREFIX=/usr/local/bin
VALGRINDFLAGS=--tool=memcheck --leak-check=full --show-leak-kinds=all -v
PROFILE_FLAGS=-fprofile-arcs -ftest-coverage
TST_LIBS=-lcheck -lm -lpthread -lrt -lsubunit
COV_LIBS=-lgcov -coverage
SRC_DIR=src
TST_DIR=tests
TST_BIN=check_$(PROGRAM_NAME)_tests
SRC_FILES=$(addprefix $(SRC_DIR)/, *.c)
LIB_FILES=$(addprefix $(SRC_DIR)/, lib*.*)
TST_FILES=$(addprefix $(TST_DIR)/, *.c)
GCOV=gcovr
GCONV_FLAGS=-r . --html --html-details

all: coverage

build_lib_objects: $(LIB_FILES)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Building object files for each library source files ..."
	$(CC) -c $(CFLAGS) $(PROFILE_FLAGS) $(LIB_FILES)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

build_tst_objects: $(TST_FILES)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Building object files for each testing source files ..."
	$(CC) -c $(CFLAGS) $(PROFILE_FLAGS) $(TST_FILES)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

$(TST_BIN): build_lib_objects build_tst_objects build
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Building $(TST_BIN) binary ..."
	$(CC) lib*.o check_$(PROGRAM_NAME).o $(TST_LIBS) $(COV_LIBS) -o $(TST_BIN)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

test: $(TST_BIN)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Running unit tests ..."
	./$(TST_BIN)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

coverage: test
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Generating code coverage report ..."
	$(GCOV) $(GCONV_FLAGS) -o coverage_report.html
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

build: $(SRC_FILES)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Building $(PROGRAM_NAME) binary ..."
	$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(SRC_FILES)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

install: build
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Installing $(PROGRAM_NAME) ..."
	install -m 0755 $(PROGRAM_NAME) $(PREFIX)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

uninstall: $(PREFIX)/$(PROGRAM_NAME)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Uninstall $(PROGRAM_NAME) from '$(PREFIX)' ..."
	rm -f $(PREFIX)/$(PROGRAM_NAME)
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

valgrind: build
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Running memory check ..."
	valgrind $(VALGRINDFLAGS) ./$(PROGRAM_NAME) -f $(FILE) -w -m $(MSG) > /dev/null
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

clean:
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Cleaning workdir ..."
	rm -f *.o *.html *.gcda *.gcno $(addprefix $(SRC_DIR)/, *.gch) *$(PROGRAM_NAME)*
	@echo "`date +'%Y/%m/%d %H:%M:%S'` [$@] Done!"

.PHONY: all build_lib_objects build_tst_objects test coverage build install uninstall valgrind clean
