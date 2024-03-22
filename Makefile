CC = gcc
CFLAGS = -Wall -Wextra --std=c11

EXEC = ipk24chat-client

ZIP_NAME = "xsnieh00.zip"

SRCDIR = src/
OBJDIR = objs/

# HEADS = $(wildcard $(SRCDIR)*.h)
HEADS = src/err_out.h src/main.h src/argv_parser.h
# SRCS = $(wildcard $(SRCDIR)*.c)
SRCS = src/err_out.c src/main.c src/argv_parser.c
# OBJS = $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRCS))
OBJS = objs/err_out.o objs/main.o objs/argv_parser.o

all: clear build

run: clear build
	./$(EXEC)

build: mkobjdir $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) $^ -r -o $@

mkobjdir:
	mkdir -p $(OBJDIR)

# valgrind:

clear:
	rm -rf $(OBJDIR)
	rm -f $(ZIP_NAME)
	rm -f $(EXEC)

zip:
	rm -f $(ZIP_NAME)
	zip $(ZIP_NAME) $(SRCS) $(HEADS) makefile readme.md manual.pdf test_cases.md