###
# Running the program:
# run `make`
# then: `./bin/assembler examples/example1.as
###

COMPILER_FLAGS = -ansi -Wall -pedantic
# Directory to put the compiler object files into
OBJ_DIR = ./obj
# Directory to put the final assembler compile program into
BIN_DIR = ./bin
# Assembler program name
BIN_FILE = $(BIN_DIR)/assembler

# Tests config, part of development only.
TESTS_DIR = ./tests
TESTS_TEMPORARY_DIR = $(TESTS_DIR)/tmp
TESTS_BIN_FILE = $(TESTS_TEMPORARY_DIR)/test_runner

#auto compile all .o files from obj dir
# todo: remove --ggdb3?
all: create_objs
	mkdir -p $(BIN_DIR)
	gcc $(COMPILER_FLAGS) $(wildcard $(OBJ_DIR)/*.o) -o $(BIN_FILE) -ggdb3
	@echo "******************************"
	@echo "**** compiled to: " $(BIN_FILE)

# create project object files
create_objs: $(wildcard *.c) $(wildcard *.h)
	mkdir -p $(OBJ_DIR)
	gcc -c $(COMPILER_FLAGS) main.c -o $(OBJ_DIR)/main.o -ggdb3
	gcc -c $(COMPILER_FLAGS) parser.c -o $(OBJ_DIR)/parser.o -ggdb3
	gcc -c $(COMPILER_FLAGS) scanners.c -o $(OBJ_DIR)/scanners.o -ggdb3
	gcc -c $(COMPILER_FLAGS) commands.c -o $(OBJ_DIR)/commands.o -ggdb3
	gcc -c $(COMPILER_FLAGS) validation.c -o $(OBJ_DIR)/validation.o -ggdb3
	gcc -c $(COMPILER_FLAGS) db.c -o $(OBJ_DIR)/db.o -ggdb3
	gcc -c $(COMPILER_FLAGS) builders.c -o $(OBJ_DIR)/builders.o -ggdb3
	gcc -c $(COMPILER_FLAGS) output.c -o $(OBJ_DIR)/output.o -ggdb3

# clean project auto-generated files
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_FILE)
	rm -f $(TESTS_TEMPORARY_DIR)/*
	rm -f $(TESTS_BIN_FILE)


# ##################################################
# Test rules. not part of project bin
# ##################################################
test: create_objs test_init test_objs
	$(TESTS_BIN_FILE)

# Create test object files
test_objs:
	gcc $(COMPILER_FLAGS) $(wildcard $(OBJ_DIR)/*.o) $(wildcard $(TESTS_TEMPORARY_DIR)/*.o) -o $(TESTS_BIN_FILE) -ggdb3

# clean previous test files and compile new testers object files
# removes main object file because we have our own main() function so we won't have duplicate
test_init:
	mkdir -p $(TESTS_TEMPORARY_DIR)
	rm -f $(TESTS_TEMPORARY_DIR)/*
	rm -f $(OBJ_DIR)/main.o
	gcc -c $(COMPILER_FLAGS) $(TESTS_DIR)/tests.c -o $(TESTS_TEMPORARY_DIR)/tests.o -ggdb3
	gcc -c $(COMPILER_FLAGS) $(TESTS_DIR)/parser-test.c -o $(TESTS_TEMPORARY_DIR)/parser-test.o -ggdb3
	gcc -c $(COMPILER_FLAGS) $(TESTS_DIR)/validation-test.c -o $(TESTS_TEMPORARY_DIR)/validation-test.o -ggdb3
	gcc -c $(COMPILER_FLAGS) $(TESTS_DIR)/db-test.c -o $(TESTS_TEMPORARY_DIR)/db-test.o -ggdb3
	gcc -c $(COMPILER_FLAGS) $(TESTS_DIR)/commands-test.c -o $(TESTS_TEMPORARY_DIR)/commands-test.o -ggdb3
	gcc -c $(COMPILER_FLAGS) $(TESTS_DIR)/output-test.c -o $(TESTS_TEMPORARY_DIR)/output-test.o -ggdb3