SRCS=main.c \
	hlib.c \
	command_get.c \
	command_free.c \
	command_init.c \
	command_split.c \
	command_exec.c \
	shell_init.c \
	shell_free.c \
	shell_env_get_index.c \
	shell_env_get.c \
	shell_env_set.c \
	shell_env_remove.c \
	shell_env_add.c \
	shell_paths_expand.c \
	shell_paths_parse.c

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
