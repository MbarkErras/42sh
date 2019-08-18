#ifndef SIMPLIST_H
# define SIMPLIST_H

# include <stdlib.h>
# include <unistd.h>

# define F_GET(x, f) (x & (1 << f))
# define F_BGET(x, f) (x & f)
# define F_SET(x, f) (x |= (1 << f))
# define F_BSET(x, f) (x |= f)
# define F_UNSET(x, f) (x &= ~(1 << f))
# define F_BUNSET(x, f) (x &= ~f)

typedef	struct	s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
	struct s_list	*prev;
}				t_list;

t_list  *list_create_node(void *content, size_t content_size);
t_list  *list_head_tail(t_list *node, int head_or_tail);
void    list_push(t_list **head, t_list *node, int X);
void    list_push_back(t_list **head, t_list *node);
void    list_push_front(t_list **head, t_list *node);
size_t	list_size(t_list *head);

#endif
