CC = gcc
LDLIBS = -lm
CFLAGS = -Wall -Wextra -Werror -std=c11

# LDLIBS += -fsanitize=address,undefined
# CFLAGS += -fsanitize=address,undefined

MODEL_SRC = $(wildcard model/*.c)
VIEW_SRC = $(wildcard view/*.c)
CONTROLLER_SRC = $(wildcard controller/*.c)
Q_LEARNING_SRC = $(wildcard q_learning/*.c)




OBJDIR = obj
SRC = main.c $(MODEL_SRC) $(VIEW_SRC) $(CONTROLLER_SRC) $(Q_LEARNING_SRC)
OBJ = $(patsubst %.c,obj/%.o,$(SRC))

TARGET = maze

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
	$(MAKE) -C tests/ clean



tests:
	$(MAKE) -C tests/ test

gcov_report:
	$(MAKE) -C tests/ coverage

valgrind:
	$(MAKE) -C tests/  valgrind-run


cl:
	find . -type f -name "*.c" -o -name "*.h"  -o -name "*.js" | xargs clang-format -i


cpp: 
	cppcheck --enable=all --force --error-exitcode=1 --std=c11 \
	--suppress=missingIncludeSystem --inconclusive \
	--language=c --check-level=exhaustive --verbose ../src



.PHONY: all tests gcov_report clean cl install uninstall valgrind maze

BINDIR := bin

install: all
	@echo "Installing to $(BINDIR)"
	mkdir -p $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)
	rm -rf ./$(TARGET)

uninstall:
	@echo "Removing from $(BINDIR)"
	rm -f $(BINDIR)/$(TARGET)
	rmdir --ignore-fail-on-non-empty $(BINDIR) || true


