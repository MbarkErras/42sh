/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utilities.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 10:40:44 by merras            #+#    #+#             */
/*   Updated: 2019/09/14 20:05:05 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

t_hist	*t_hist_construct(t_hist entry)
{
	t_hist *node;

	node = malloc(sizeof(t_hist));
	node->value = entry.value;
	node->fever = ft_strdup(entry.value);
	node->date = entry.date;
	node->flags = 0;
	return (node);
}

void	ack_history_change(t_list *node)
{
	if (!node)
		return ;
	if (!ft_strequ(((t_hist *)(node->content))->value, ((t_hist *)(node->content))->fever))
		F_SET(((t_hist *)(node->content))->flags, F_CHANGED);
}

void	read_history_resetting(void)
{
	if (!rd_config_getter(NULL)->editing_history_node)
		return ;
	((t_hist *)(rd_config_getter(NULL)->editing_history_node->content))->value = ((t_hist *)(rd_config_getter(NULL)->editing_history_node->content))->fever;
	F_UNSET(((t_hist *)(rd_config_getter(NULL)->editing_history_node->content))->flags, F_CHANGED);
}

int		print_history()
{
	t_list		*history;
	t_3tuple	sizes;

	history = sh_config_getter(NULL)->hist;
	sizes.i = 0;
	sizes.j = ft_digitscount((long long int)list_size(history));
	while (history && ++sizes.i)
	{
		sizes.k = sizes.j - ft_digitscount(sizes.i) + 2;
		while (sizes.k--)
			ft_putchar(' ');
		ft_putnbr(sizes.i);
		ft_putstr(F_GET(((t_hist *)history->content)->flags, F_CHANGED) ? "* " : "  ");
		ft_putendl(((t_hist *)history->content)->value);
		history = history->next;	
	}
	return (1);
}

void	hist_node_del(void *n)
{
	t_hist *node = (t_hist *)n;
	ft_strdel(&node->value);
	ft_strdel(&node->fever);
	free(node);
}
