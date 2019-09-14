#include "mshell.h"

int	history_p(char **in)
{
	t_list **head;

	if (!in)
		return (-1);
	while (*in)
	{
		ft_putendl(*in);
		in++;
	}
	head = &sh_config_getter(NULL)->hist;
	list_delete_node(*head, 
			list_pointer_address(
				head,
				list_head_tail(*head, 1)),
			hist_node_del);
	return (0);
}

int	history_s(char **in)
{
	t_list **head;
	char *add;

	if (!in)
		return (-1);
	add = NULL:
	while (*in)
	{
		ft_strjoin_free(add, *in, 1);
		in++;
	}
	head = &sh_config_getter(NULL)->hist;
	list_delete_node(*head, 
			list_pointer_address(
				head,
				list_head_tail(*head, 1)),
			hist_node_del);
	list_push_back(head, list_create_node(t_hist_construct((t_hist){add, time(NULL), NULL, 2}), sizeof(t_hist)));
	return (0);
}

