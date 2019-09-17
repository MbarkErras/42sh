NAME = 21sh
SRC = src
BUILD = .build
INC = includes
LIB = libft

SRC_BUILTIN = $(patsubst %.c, $(SRC)/builtins/%.c, b_cd.c builtins.c builtins_drivers.c  env_drivers.c error_handling.c is_delimited.c variables.c)
SRC_EXECUTION = $(patsubst %.c, $(SRC)/execution/%.c, execution.c job_control.c redirections.c)
SRC_EXPANSION = $(patsubst %.c, $(SRC)/expansions/%.c, escape_god.c expansions.c)
SRC_HISTORY = $(patsubst %.c, $(SRC)/history/%.c, history.c history_file_operations.c history_operations.c history_ps.c history_utilities.c utils.c)
SRC_LINEEDITING = $(patsubst %.c, $(SRC)/line_editing/%.c, autocompletion.c clipboard.c command_line_editor.c cursor_motion.c dispatchers.c move_line.c move_word.c read_input.c terminal.c)
SRC_PARSING = $(patsubst %.c, $(SRC)/parsing/%.c, build_parse_list.c parse.c parse_heredoc.c parse_redir.c parse_redir2.c quoting.c syntax_checking.c)
SRC_SHELL = $(patsubst %.c, $(SRC)/shell/%.c, cleanup.c mshell.c)

OBJ_BUILTIN = $(patsubst %.c, %.o, $(SRC_BUILTIN))
OBJ_EXECUTION = $(patsubst %.c, %.o, $(SRC_EXECUTION))
OBJ_EXPANSIONS = $(patsubst %.c, %.o, $(SRC_EXPANSION))
OBJ_HISTORY = $(patsubst %.c, %.o, $(SRC_HISTORY))
OBJ_LINEEDITING = $(patsubst %.c, %.o, $(SRC_LINEEDITING))
OBJ_PARSING = $(patsubst %.c, %.o, $(SRC_PARSING))
OBJ_SHELL = $(patsubst %.c, %.o, $(SRC_SHELL))

OBJECT = $(OBJ_BUILTIN) $(OBJ_EXECUTION) $(OBJ_EXPANSIONS) $(OBJ_HISTORY) $(OBJ_LINEEDITING) $(OBJ_PARSING) $(OBJ_SHELL)
OBJ_DIR = $(patsubst %, $(BUILD)/%, /$(notdir $(OBJECT)))
CC = gcc
FLAGS = -g #-Wall -Wextra -Werror
INCLUDE = -I$(INC) -I$(LIB)/includes
LIBFT = $(LIB)/libft.a
SIMPLIST_DIR = simplist

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECT) simplist/simplist.c
	$(CC) $(FLAGS) $(OBJ_DIR) -ltermcap $(LIBFT) $(SIMPLIST_DIR)/simplist.a -o $(NAME)

$(LIBFT):
	make -C $(LIB)
	make -C $(SIMPLIST_DIR)

%.o : %.c | $(BUILD)
	$(CC) $(FLAGS) $(INCLUDE) -I$(SIMPLIST_DIR) -c $< -o $(BUILD)/$(notdir $@)

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)
	rm -rf build
	make -C $(LIB) clean
	make -C $(SIMPLIST_DIR) clean

fclean: clean
	rm -rf $(NAME)
	make -C $(LIB) fclean
	make -C $(SIMPLIST_DIR) fclean

re : fclean all