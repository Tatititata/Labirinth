CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
CFLAGS += -fsanitize=address,undefined -g
MODEL_SRC = $(wildcard model/*.c)
VIEW_SRC = $(wildcard view/*.c)
CONTROLLER_SRC = $(wildcard controller/*.c)
Q_LEARNING_SRC = $(wildcard q_learning/*.c)

LDLIBS = -lm

OBJDIR = obj
SRC = main.c $(MODEL_SRC) $(VIEW_SRC) $(CONTROLLER_SRC) $(Q_LEARNING_SRC)
OBJ = $(patsubst %.c,obj/%.o,$(SRC))


TARGET = maze

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

install:
	cp $(TARGET) /usr/local/bin

uninstall:
	rm /usr/local/bin/$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

tests:

cl:
	find . -type f -name "*.c" -o -name "*.h" | xargs clang-format -i

valgrind: $(TARGET)
	valgrind \
    --leak-check=full \
    --track-origins=yes \
    --show-leak-kinds=all \
    --trace-children=yes \
	-s \
    ./$(TARGET)