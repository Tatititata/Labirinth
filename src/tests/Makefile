CC := gcc

COMMON_FLAGS := -Wall -Wextra -Werror -std=c11 -I. -g -DTESTING
LDLIBS := -lcheck -lsubunit -lrt -lpthread -lm

SAN_FLAGS := -fsanitize=address,undefined

COVERAGE_FLAGS := -fprofile-arcs -ftest-coverage
COVERAGE_INFO := coverage.info
COVERAGE_REPORT_DIR := ../report

MODEL_SRC := \
    ../model/cave.c \
    ../model/maze_eller.c \
    ../model/maze_solver.c \
	../model/parser.c \
	../q_learning/learning.c

TEST_SRC := $(wildcard *.c)

MODEL_OBJS := $(notdir $(MODEL_SRC:.c=.o))
TEST_OBJS := $(TEST_SRC:.c=.o)
OBJS := $(MODEL_OBJS) $(TEST_OBJS)

TARGET := test_exec

all: sanitizer

sanitizer: CFLAGS := $(COMMON_FLAGS) $(SAN_FLAGS)
sanitizer: LDFLAGS := $(SAN_FLAGS)
sanitizer: clean $(TARGET)

valgrind: CFLAGS := $(COMMON_FLAGS)
valgrind: LDFLAGS :=
valgrind: clean $(TARGET)

coverage: CFLAGS := $(COMMON_FLAGS) $(COVERAGE_FLAGS)
coverage: LDFLAGS := $(COVERAGE_FLAGS)
coverage: LDLIBS += -lgcov
coverage: clean $(TARGET)
	./$(TARGET)
	mkdir -p $(COVERAGE_REPORT_DIR)
	gcovr --root .. --filter ../model/ --filter ../q_learning/ \
	--html-details $(COVERAGE_REPORT_DIR)/report.html --exclude-branches-by-pattern '.*'

test: sanitizer
	./$(TARGET)

valgrind-run: valgrind
	valgrind \
		--leak-check=full \
		--track-origins=yes \
		--show-leak-kinds=all \
		--trace-children=yes \
		-s \
		./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: ../model/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: ../q_learning/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


clean: clean-coverage
	rm -f $(OBJS) $(TARGET)

clean-coverage:
	rm -rf $(COVERAGE_REPORT_DIR) *.gcov *gcno *gcda

.PHONY: all sanitizer valgrind coverage test valgrind-run clean clean-coverage
