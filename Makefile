.POSIX:
.SUFFIXES:

CC := gcc
C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
CFLAGS := -c -std=c99 -pedantic -Wall -g
LDFLAGS := -lm
RM := rm -f
MKDIR := mkdir -p
NAME := tmath

all: obj/ $(NAME) 

obj/:
	$(MKDIR) $@

$(NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY:
clean:
	$(RM) obj/*.o $(NAME)

