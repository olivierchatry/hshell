SRCS=main.c \
	hlib.c \
	command_get.c \
	command_free.c \
	command_init.c \
	command_split.c \
	command_exec.c \
	command_builtins.c \
	shell_init.c \
	shell_free.c \
	env_get_index.c \
	env_get.c \
	env_set.c \
	env_remove.c \
	env_add.c \
	env_hook.c \
	env_rebuild_envp.c \
	env_free_envp.c \
	paths_expand.c \
	paths_parse.c \
	paths_free.c

OBJS=$(SRCS:.c=.o)
TARGET=shell
CFLAGS=-I. -g -O0 -Wall -Werror -Wextra -pedantic
RM=rm -f

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC)  $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o $(TARGET)
