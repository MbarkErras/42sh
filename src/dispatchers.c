/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatchers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 09:27:18 by merras            #+#    #+#             */
/*   Updated: 2019/07/31 09:19:26 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	terminal_delete_dispatcher(t_read *rd)
{
	if (rd->b[0] == DEL && rd->pos != rd->prmpt)
		terminal_delete(rd, 1);
	else if (rd->b[0] == CTRLD)
		terminal_delete_forward(rd);
}

void	cursor_move_dispatcher(t_read *rd)
{
	if (WORD_MOTION(rd->b[0]))
		move_word(rd);
	else if (LINE_MOTION(rd->b[0]))
		move_line(rd);
	else if (ARW(rd->b[0]))
		move_character(rd);
	else if (END_HOME(rd->b[0]))
		end_home(rd);
}

void	cboard_dispatcher(t_read *rd)
{
	if (rd->b[0] == CB_PASTE)
		terminal_insert(rd, rd->cboard);
	else
		clipboard_copy_cut(rd);
}

void	move_left_wrap(t_read *rd)
{
	int i;

	i = current_segment_size(*(rd->in), '\n', rd->pos - rd->prmpt - 1);
	i = i % rd->term.ws_col;
	i = rd->r == 0 ? i + rd->prmpt + 1 : i;
	if (i)
		tputs(tparm(tgetstr("RI", NULL), i), 1, ft_putchar_int);
	rd->c = i + 1;
}
