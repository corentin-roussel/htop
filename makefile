CC = gcc
SRCS = main.c application.c display.c process.c ui.c utils.c
TARGET = htop
OBJ_DIR =obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

LDFLAGS = -L/usr/lib/x86_64-linux-gnu
ifeq ($(OS),Windows_NT)
	LDFLAGS += -lncursesw
else
	LDFLAGS += -lncurses
endif
INCLUDES = -I/usr/include
CFLAGS = -Wall -Wextra -Werror -g3 $(INCLUDES)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

f-clean: clean
	rm -f $(TARGET)

run: $(TARGET)
	./$^ $(RUN_ARGS)