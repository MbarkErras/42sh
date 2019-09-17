/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 22:53:57 by merras            #+#    #+#             */
/*   Updated: 2019/08/03 16:46:40 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	move_line_up(t_read *rd)
{
	int temp;
	int i;

	temp = rd->c;
	i = temp + 2;
	while (--i && rd->pos > rd->prmpt)
		move_left(rd);
	if (rd->c <= temp)
		return ;
	else
	{
		while (rd->c > temp && rd->pos > rd->prmpt)
			move_left(rd);
	}
}

void	move_line_down(t_read *rd)
{
	int temp;
	int i;

	temp = rd->c;
	i = rd->r;
	while (rd->r != i + 1 && rd->pos < rd->prmpt + (int)ft_strlen(*(rd->in)))
		move_right(rd);
	while (rd->pos < rd->prmpt + (int)ft_strlen(*(rd->in)) &&
			(*(rd->in))[rd->pos - rd->prmpt + 1] != '\n')
	{
		if (rd->c == temp)
			return ;
		move_right(rd);
	}
}

void	move_line(t_read *rd)
{
	if (rd->b[0] == MOVE_UP_LINE)
		move_line_up(rd);
	else if (rd->b[0] == MOVE_DOWN_LINE)
		move_line_down(rd);
}
