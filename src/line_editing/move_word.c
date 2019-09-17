/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 23:11:07 by merras            #+#    #+#             */
/*   Updated: 2019/08/03 16:47:00 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

static void	move_word_left(t_read *rd)
{
	int flag;

	flag = 0;
	while (rd->pos > rd->prmpt)
	{
		if (WORD_DELIMITER((*rd->in)[rd->pos - rd->prmpt]) && flag == 2)
			return (move_right(rd));
		if (WORD_DELIMITER((*rd->in)[rd->pos - rd->prmpt]))
			flag = 1;
		if (!WORD_DELIMITER((*rd->in)[rd->pos - rd->prmpt]) && flag)
			flag = 2;
		move_left(rd);
	}
}

static void	move_word_right(t_read *rd)
{
	int flag;

	flag = 0;
	while (rd->pos < rd->prmpt + (int)ft_strlen(*(rd->in)))
	{
		if (WORD_DELIMITER((*(rd->in))[rd->pos - rd->prmpt]))
			flag = 1;
		move_right(rd);
		if (!WORD_DELIMITER((*(rd->in))[rd->pos - rd->prmpt]) && flag)
			return ;
	}
}

void		move_word(t_read *rd)
{
	if (rd->b[0] == MOVE_WORD_LEFT)
		move_word_left(rd);
	else if (rd->b[0] == MOVE_WORD_RIGHT)
		move_word_right(rd);
}
