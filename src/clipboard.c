/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipboard.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 21:31:00 by merras            #+#    #+#             */
/*   Updated: 2019/08/17 13:22:01 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	clipboard_copy_cut(t_read *rd)
{
	if ((rd->flags & 8) == 8)
	{
		rd->flags &= ~8;
		rd->cb_end = rd->pos - rd->prmpt;
		ft_strdel(&rd->cboard);
		rd->cboard =
			ft_strsub(*(rd->in), rd->cb_start, rd->cb_end - rd->cb_start);
		if (rd->b[0] == 'c')
		{
			terminal_delete(rd, rd->cb_end - rd->cb_start);
			terminal_insert(rd, rd->cboard);
		}
		if (rd->b[0] == 'x')
			terminal_delete(rd, rd->cb_end - rd->cb_start);
	}
	else
	{
		rd->flags |= 8;
		rd->cb_start = rd->pos - rd->prmpt;
	}
}

void	get_left(t_read *rd)
{
	while (rd->pos > rd->prmpt)
		move_left(rd);
}

void	get_right(t_read *rd)
{
	while (rd->pos < (int)(rd->prmpt + ft_strlen(*rd->in)))
		move_right(rd);
}

void	free_and_init(char **ptr)
{
	ft_strdel(ptr);
	*ptr = ft_strnew(0);
}
