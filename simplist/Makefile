NAME = simplist.a

SRCS = simplist.c
OBJS = $(SRCS:.c=.o)
INCLUDES = simplist.h

FLAGS = -Wextra -Werror -Wall

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

$(OBJS): %.o : %.c
	gcc $(FLAGS) -c $< -o $@


clean:
	rm -f  $(NAME)

fclean: clean

re: fclean all
