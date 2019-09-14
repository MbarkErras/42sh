#include "mshell.h"

int	history_drange(int *start, int *end, char *arg)
{
	char *dash;

	if ((dash = ft_strchr(arg + 1, '-') && dash != arg + 1))
	{
		if (arg[0] != '-' && !ft_isdigit(arg[0]))
			return (0);
		if (!ft_isnumber(dash + 1))
			return (0);
		*end = ft_atoi(dash + 1);
		*dash = '\0';
		if (!ft_isnumber(arg))
			return (0);
		*start = ft_atoi(arg);
		return (1);
	}
	else
	{
		if (!ft_isnumber(arg))
			return (0);
		*start = ft_atoi(arg);
		*end = *start;
		return (1);
	}
}

int	history_delete(char **in)
{
	int start;
	int end;
	t_list **head;
	size_t size;

	if (!n[0])
		return (ft_perror(EXEC_NAME, NULL, N_ARG)); //no args
	if (in[1])
		return (ft_perror(EXEC_NAME, NULL, X_ARG)); //too many args
	if (history_drange(&start, &end, in[0]))
		return (ft_perror(EXEC_NAME, NULL, H_PRM)); //not valid range
	if ((start = POSI(start, size)) > (end = POSI(end, size)))
		return (ft_perror(EXEC_NAME, NULL, H_PRM)); //not valid range
	head = &sh_config_getter(NULL)->hist;
	size = list_size(*head);
	list_delete_range(*head,
			list_pointer_address(head,
				list_indexed_node(*head, start)),
			list_pointer_address(head,
				list_indexed_node(*head, end)),
			del_node_hist);
	return (0);
}

int	hist_new(t_hist *entry)
{
	if (F_GET(entry->flags, F_NEW))
		return (1);
	return (0);
}

int	history_write(char **in, char c)
{
	char	*fn;
	int	fd;
	t_list	*head;

	if (!in[0])
	{
		if (fn = read_env("HISTFILE"))
			return (ft_perror(EXEC_NAME, NULL, N_PRM)); //HISTFILE not set
	}
	else
	{
		if (in[1])
			return (ft_perror(EXEC_NAME, NULL, X_ARG)); //too many args
		fn = in[0];
	}
	if ((fd = open(fn, c == 'a' ? O_APPEND : O_WRITE) == -1))
			return (ft_perror(EXEC_NAME, NULL, B_HFL)); //open error
	head = sh_config_getter(NULL)->hist;
	head = c == 'a' ? list_find_node(head, hist_new) : head;
	while (head)
	{
		fn = ((t_hist *)(head->content))->value;
		write (fd, fn, ft_strlen(fn));
		head = head->next;
	}
	return (1);
}

int	history_read(char **in, char c)
{
	int fn;
	char *history_content;
	char **new;
	int i;

	if (in[0])
	{
		if (fn = read_env("HISTFILE"))
			return (ft_perror(EXEC_NAME, NULL, X_ARG)); //too many args	
	}
	if (fn = read_env("HISTFILE"))
		return (ft_perror(EXEC_NAME, NULL, N_PRM)); //HISTFILE not set
	if (history_content = file_reader(fn))
		return (-1);
	list_delete(sh_config_getter(NULL)->hist, hist_node_del);
	new = ft_strsplit(history_content, '\n');
	i = 0;
	while (new[i])
		list_push_back(&sh_config_getter(NULL)->hist, list_create_node(t_hist_construct((t_hist){new[i], time(NULL), NULL, 0}), sizeof(t_hist)));
	return (0);

}
