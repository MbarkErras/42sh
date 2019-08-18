NAME=21sh

SRCS=b_cd.c \
	 builtins.c \
	 builtins_drivers.c \
	 env_drivers.c \
	 execution.c \
	 is_delimited.c \
	 clipboard.c \
	 terminal.c \
	 command_line_editor.c \
	 cursor_motion.c \
	 dispatchers.c \
	 move_line.c \
	 move_word.c \
	 read_input.c \
	 cleanup.c \
	 error_handling.c \
	 mshell.c \
	 history_utilities.c \
	 history.c \
	 redirections.c

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


