# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/06/08 14:43:13 by merras            #+#    #+#              #
#    Updated: 2019/08/09 01:35:29 by yoyassin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
	 parse.c \
	 parse_redir.c \
	 parse_redir2.c \
	 cleanup.c \
	 error_handling.c \
	 mshell.c \
	 expansions.c \
	 redirections.c \

OBJS=$(SRCS:.c=.o)

OBJS_DIR=build
SRCS_DIR=src
LIBFT_DIR=libft
OBJS_PATH=$(addprefix $(OBJS_DIR)/, $(OBJS))
SRCS_PATH=$(addprefix $(SRCS_DIR)/, $(SRCS))
INCLUDES=includes

FLAGS= -g #-Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS_PATH) $(INCLUDES)/mshell.h
	make -C $(LIBFT_DIR)
	gcc $(FLAGS) -ltermcap $(LIBFT_DIR)/libft.a $(OBJS_PATH) -o $(NAME)

$(OBJS_PATH): $(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c | $(OBJS_DIR)
	gcc $(FLAGS) -I$(INCLUDES) -I$(LIBFT_DIR)/includes -c $< -o $@

$(OBJS_DIR):
	mkdir $(OBJS_DIR)

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	rm -rf $(NAME)

re: fclean all
	make -C $(LIBFT_DIR) re
