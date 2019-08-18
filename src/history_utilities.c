/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utilities.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/16 10:40:44 by merras            #+#    #+#             */
/*   Updated: 2019/08/17 17:56:20 by merras           ###   ########.fr       */
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
	return (node);
}

void	ack_history_change(t_list *node)
{
	if (!node)
		return ;
	if (!ft_strequ(((t_hist *)(node->content))->value, ((t_hist *)(node->content))->fever))
		((t_hist *)(node->content))->changed = 1;
}

void	read_history_resetting(void)
{
	if (!rd_config_getter(NULL)->editing_history_node)
		return ;
	return ;
	((t_hist *)(rd_config_getter(NULL)->editing_history_node->content))->value = ((t_hist *)(rd_config_getter(NULL)->editing_history_node->content))->fever;
	((t_hist *)(rd_config_getter(NULL)->editing_history_node->content))->changed = 0;
}
