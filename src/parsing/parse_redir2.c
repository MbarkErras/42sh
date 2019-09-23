/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 00:55:29 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/23 22:36:27 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mshell.h"

int		get_redir_fds(t_redir *curr, char *str, int *i)
{
	int	old_i;
	int	j;

	old_i = *i;
	j = *i;
	if (str[*i - 1 >= 0 ? *i - 1 : 0] == BLANK
	|| ft_isalpha(str[(*i - 1 > 0) ? *i - 1 : 0]))
		curr->src_fd = curr->type == O_RDONLY ? 0 : 1;
	else
	{
		while (str[*i] && ft_isdigit(str[(--(*i) > 0) ? *i : 0]))
			;
		curr->src_fd = ft_atoi(str + *i + 1);
		j = *i;
	}
	*i = (curr->type == FD_AGGR) ? old_i + 2 : old_i;
	if (curr->type == FD_AGGR && ft_isdigit(str[*i]))
	{
		*i = old_i + 2;
		while (str[*i] && ft_isdigit(str[*i]))
			(*i)++;
		curr->dst_fd = ft_atoi(str + old_i + 2);
	}
	ft_memset((void *)str + ((j > 0) ? j : 0), BLANK, *i - j);
	return ((curr->type == FD_AGGR) ? 0 : 1);
}

int		get_filename_len(char *str, int *i, int *old_i)
{
	int	length;

	length = 0;
	while (str[*i] == BLANK)
	{
		(*old_i)++;
		(*i)++;
	}
	while (ft_isalnum(str[*i]))
	{
		length++;
		(*i)++;
	}
	(*i)--;
	return (length);
}

void	get_redir_file(t_redir *curr, char *str, int *i)
{
	int	length;
	int	old_i;

	str[*i] = BLANK;
	if (curr->type == O_APPEND)
	{
		str[*i + 1] = BLANK;
		(*i)++;
	}
	old_i = *i;
	length = get_filename_len(str, i, &old_i);
	curr->file = ft_strsub(str, old_i, length);
	ft_memset(str + old_i, BLANK, length);
}
