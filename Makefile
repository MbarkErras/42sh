NAME = 21sh
SRC = src
BUILD = .build
INC = includes
HDRS = includes/mshell.h
LIB = libft
CC = gcc
FLAGS = -g #-Wall -Wextra -Werror
INCLUDE = -I$(INC) -I$(LIB)/includes
LIBFT = $(LIB)/libft.a
SIMPLIST_DIR = simplist
SRC_SUBDIRS = builtins execution expansions history line_editing parsing shell

SRC_BUILTIN = $(addprefix builtins/, b_cd.c builtins.c builtins_drivers.c  env_drivers.c error_handling.c is_delimited.c variables.c)
SRC_EXECUTION = $(addprefix execution/, execution.c job_control.c redirections.c)
SRC_EXPANSION = $(addprefix expansions/, globbing.c escape_god.c expansions.c)
SRC_HISTORY = $(addprefix history/, history.c history_file_operations.c history_operations.c history_ps.c history_utilities.c utils.c)
SRC_LINEEDITING = $(addprefix line_editing/, search.c autocompletion.c clipboard.c command_line_editor.c cursor_motion.c dispatchers.c move_line.c move_word.c read_input.c terminal.c)
SRC_PARSING = $(addprefix parsing/, build_job_list.c build_parse_list.c parse.c parse_heredoc.c parse_redir.c parse_redir2.c quoting.c syntax_checking.c utility_funcs.c)
SRC_SHELL = $(addprefix shell/, cleanup.c mshell.c)
SRCS = $(SRC_BUILTIN) $(SRC_EXECUTION) $(SRC_EXPANSION) $(SRC_HISTORY) $(SRC_LINEEDITING) $(SRC_PARSING) $(SRC_SHELL)
OBJECT = $(addprefix $(BUILD)/, $(SRCS:.c=.o))
OBJ_DIRS = $(addprefix $(BUILD)/, $(SRC_SUBDIRS))

# all:
# 	$(info $$OBJ_DIRS is [${OBJ_DIRS}])

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECT) simplist/simplist.c
	$(CC) $(FLAGS) $(OBJECT) $(INCLUDE) -ltermcap $(LIBFT) $(SIMPLIST_DIR)/simplist.a -o $(NAME)

$(OBJECT): $(BUILD)/%.o : $(SRC)/%.c $(HDRS) | $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) -I$(SIMPLIST_DIR) -c $< -o $@

$(BUILD):
	mkdir $(BUILD)
	mkdir $(OBJ_DIRS)

$(LIBFT):
	make -C $(LIB)
	make -C $(SIMPLIST_DIR)

clean:
	rm -rf $(BUILD)
	make -C $(LIB) clean
	make -C $(SIMPLIST_DIR) clean

fclean: clean
	rm -rf $(NAME)
	make -C $(LIB) fclean
	make -C $(SIMPLIST_DIR) fclean

re : fclean all
