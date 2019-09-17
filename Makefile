NAME=21sh

SRCS=autocompletion.c \
			b_cd.c \
			builtins.c \
			builtins_drivers.c \
			cleanup.c \
			clipboard.c \
			command_line_editor.c \
			cursor_motion.c \
			dispatchers.c \
			env_drivers.c \
			error_handling.c \
			escape_god.c \
			execution.c \
			expansions.c \
			history.c\
			history_file_operations.c \
			history_operations.c \
			history_ps.c \
			history_utilities.c \
			utils.c \
			is_delimited.c \
			job_control.c \
			move_line.c \
			move_word.c \
			mshell.c \
			build_parse_list.c \
			parse.c \
			syntax_checking.c \
			quoting.c \
			parse_heredoc.c \
			parse_redir2.c \
			parse_redir.c \
			read_input.c \
			redirections.c \
			terminal.c \
			variables.c \

OBJS=$(SRCS:.c=.o)

OBJS_DIR=build
SRCS_DIR=src
LIBFT_DIR=libft
SIMPLIST_DIR=simplist
OBJS_PATH=$(addprefix $(OBJS_DIR)/, $(OBJS))
SRCS_PATH=$(addprefix $(SRCS_DIR)/, $(SRCS))
INCLUDES=includes

FLAGS= -g #-Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS_PATH) $(INCLUDES)/mshell.h simplist/simplist.c # last dep is temp
	make -C $(LIBFT_DIR)
	make -C $(SIMPLIST_DIR)
	gcc $(FLAGS) $(OBJS_PATH) -ltermcap $(LIBFT_DIR)/libft.a $(SIMPLIST_DIR)/simplist.a  -o $(NAME)

$(OBJS_PATH): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c | $(OBJS_DIR)
	gcc $(FLAGS) -I$(INCLUDES) -I$(LIBFT_DIR)/includes -I$(SIMPLIST_DIR) -c $< -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

clean:
	make -C $(LIBFT_DIR) clean
	make -C $(SIMPLIST_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	make -C $(SIMPLIST_DIR) fclean
	rm -rf $(NAME)

re: relib fclean all

relib:
	make -C $(LIBFT_DIR) re
	make -C $(SIMPLIST_DIR) re
