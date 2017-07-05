BUILD_DIR = ./build
TARGET=hsh

SRCS=src/main.c \
	src/command/command_get.c \
	src/command/command_free.c \
	src/command/command_init.c \
	src/command/command_exec.c \
	src/command/command_builtins.c \
	src/command/command_lexer.c \
	src/command/command_expand.c \
	src/command/command_remove_comment.c \
	src/command/command_remove_quote.c \
	src/command/command_clone.c \
	src/command/command_redirections.c \
	src/shell/shell_init.c \
	src/shell/shell_free.c \
	src/shell/shell_getcwd.c \
	src/alias/alias_get_index.c \
	src/alias/alias_get.c \
	src/alias/alias_set.c \
	src/alias/alias_add.c \
	src/alias/alias_expand.c \
	src/env/env_get_index.c \
	src/env/env_get.c \
	src/env/env_set.c \
	src/env/env_remove.c \
	src/env/env_add.c \
	src/env/env_hook.c \
	src/env/env_rebuild_envp.c \
	src/env/env_free_envp.c \
	src/env/env_expand.c \
	src/path/path_expand.c \
	src/path/path_parse.c \
	src/path/path_free.c \
	src/history/history_add.c \
	src/history/history_init.c \
	src/history/history_free.c \
	src/history/history_expand.c \
	src/history/history_save.c \
	src/history/history_load.c \
	src/history/history_get_file.c \
	src/utils/hlib.c \
	src/utils/util_get_home.c \
	src/utils/util_read_file.c \
	src/utils/util_get_user.c \
	src/utils/util_get_hostname.c \
	src/prompt/prompt_print.c \
	src/prompt/prompt_expand.c 

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
DEPS=$(OBJS:%.o=%.d)
CFLAGS=-I. -I./src -g -O0 -Wall -Werror -Wextra -pedantic
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
	$(RM) -r $(BUILD_DIR)

.PHONY	: re
re	: clean all
