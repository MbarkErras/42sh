/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 21:31:42 by merras            #+#    #+#             */
/*   Updated: 2019/09/24 12:28:43 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

int	history_dispatcher(char c, char **in)
{
	if (c == 'c')
		list_delete(sh_config_getter(NULL)->hist, hist_node_del);
	else if (c == 'd')
		return (history_delete(in));
	else if (c == 'a' || c == 'w')
		return (history_write(in, c));
	else if (c == 'r' || c == 'n')
		return (0);
	else if (c == 'p')
		return (1);
	else if (c == 's')
		return (2);
	return (0);
}

int	history_turing_iterator(char *flagset, char **in)
{
	int ret;

	while (*flagset)
	{
		if ((ret = history_dispatcher(*flagset, in)))
			return (ft_perror(EXEC_NAME, NULL, 0));
		flagset++;
	}
	return (0);
}

char    *search_in_history(t_list *head, int to_find)
{
	head = list_indexed_node(head, POSI(to_find, list_size(head)));
	if (!head)
		return (NULL);
	return (((t_hist *)head->content)->value);
}

int		word_search_filter(void *n, void *p)
{
	t_list *node;
	char	*strstr;

	node = (t_list *)n;
	return ((strstr = ft_strstr(((t_hist *)node->content)->value, p)) == ((t_hist *)node->content)->value ? 1 : 0);
}

char    *result_giving(char *to_search)
{
	char    *his_cmd;
	t_list	*his;
	t_list *node;

	his = sh_config_getter(NULL)->hist;
	if (to_search[0] != '!')
		return (NULL);
	if (to_search[1] == '!' && to_search[2] == '\0')
	{
		his = list_head_tail(his, 1);
		return (((t_hist *)his->content)->value);
	}
	his_cmd = ft_strsub(to_search, 1, ft_strlen(to_search) - 1);
	if (ft_isnumber(his_cmd))
		return (search_in_history(his, ft_atoi(his_cmd)));
	else
	{
		if ((node = list_find_node(his, word_search_filter, his_cmd)))
			return (((t_hist *)node->content)->value);
	}
	return (NULL);
}


int	b_history(char **in)
{
	int i;
	int j;
	char flagset[9];

	if (!in[1])
		return (print_history());
	i = 0;
	ft_memset(flagset, 0, sizeof(flagset));
	while (in[++i][0] == '-')
	{
		if (!is_flag(in[i] + 1, "cdanrwps"))
			return (ft_perror(EXEC_NAME, in[i], N_PRM));
		setter(flagset, in[i] + 1);
	}
	return (history_turing_iterator(flagset, in + i));
}
