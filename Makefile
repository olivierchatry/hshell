BUILD_DIR = ./build
TARGET=simple_shell

SRCS=main.c \
	hlib.c \
	command_get.c \
	command_free.c \
	command_init.c \
	command_exec.c \
	command_builtins.c \
	command_lexer.c \
	command_expand.c \
	command_remove_comment.c \
	command_remove_quote.c \
	command_clone.c \
	shell_init.c \
	shell_free.c \
	shell_getcwd.c \
	alias_get_index.c \
	alias_get.c \
	alias_set.c \
	alias_add.c \
	alias_expand.c \
	env_get_index.c \
	env_get.c \
	env_set.c \
	env_remove.c \
	env_add.c \
	env_hook.c \
	env_rebuild_envp.c \
	env_free_envp.c \
	env_expand.c \
	paths_expand.c \
	paths_parse.c \
	paths_free.c \
	history_add.c \
	history_init.c \
	history_free.c \
	history_expand.c \
	history_save.c \
	history_load.c \
	history_get_file.c \
	util_get_home.c \
	util_read_file.c \
	util_get_user.c \
	util_get_hostname.c \
	prompt_print.c \
	prompt_expand.c 

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS=$(OBJS:%.o=%.d)
CFLAGS=-I. -g -O0 -Wall -Werror -Wextra -pedantic
RM=rm -f

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC)  $(CFLAGS) $(INCLUDES) -o $(TARGET) $(OBJS) $(LFLAGS) $(LIBS)

-include $(DEPS)

$(BUILD_DIR)/%.o : %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

.PHONY : clean
clean:
	$(RM) $(OBJS) $(DEPS) $(TARGET) vgcore*
