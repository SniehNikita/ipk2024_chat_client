# @file makefile
# @author Sniehovskyi Nikita (xsnieh00)
# @date 16.03.2024
# @brief Builds project. Use 'make' to build

CC = gcc
CFLAGS = -Wall -Wextra --std=c11

EXEC = ipk24chat-client

ZIP_NAME = "xsnieh00.zip"

SRCDIR = src/
OBJDIR = objs/

# HEADS = $(wildcard $(SRCDIR)*.h)
HEADS = src/err_out.h src/main.h src/argv_parser.h src/client.h src/msg_parse.h src/msg_compose.h src/command.h src/queue.h src/state.h src/types.h
# SRCS = $(wildcard $(SRCDIR)*.c)
SRCS = src/err_out.c src/main.c src/argv_parser.c src/client.c src/msg_parse.c src/msg_compose.c src/command.c src/queue.c src/state.c
# OBJS = $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRCS))
OBJS = objs/err_out.o objs/main.o objs/argv_parser.o objs/client.o objs/msg_parse.o objs/msg_compose.o objs/command.o objs/queue.o objs/state.o

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

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./ipk24chat-client -t udp -s anton5.fit.vutbr.cz -p 4567 -d 250 -r 3

clear:
	rm -rf $(OBJDIR)
	rm -f $(ZIP_NAME)
	rm -f $(EXEC)

zip:
	rm -f $(ZIP_NAME)
	zip $(ZIP_NAME) $(SRCS) $(HEADS) Makefile README.md CHANGELOG.md LICENSE