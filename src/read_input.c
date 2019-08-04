/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 15:56:54 by merras            #+#    #+#             */
/*   Updated: 2019/08/03 16:47:29 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

static void		submit_cline(t_read *rd)
{
	while (rd->pos < rd->prmpt + (int)ft_strlen(*rd->in))
		move_right(rd);
	ft_putchar('\n');
	F_UNSET(rd->flags, F_RDHALT);
}

t_read			*rd_config_getter(t_read *rd_set)
{
	static t_read *rd_get;

	if (rd_set)
		rd_get = rd_set;
	return (rd_get);
}

t_read			init_read(char *prompt, t_string *hist, char *cboard, char **in)
{
	t_read		rd;
	t_string	*tail;

	tail = hist;
	while (tail && tail->next)
		tail = tail->next;
	rd.hist = tail;
	*in = ft_strnew(0);
	rd.in = in;
	rd.cboard = cboard;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &(rd.term));
	rd.flags = 0;
	rd.prmpt = ft_strlen(prompt);
	rd.pos = rd.prmpt;
	rd.c = rd.prmpt;
	rd.r = 0;
	rd.b[1] = '\0';
	rd.flags = 0;
	F_SET(rd.flags, F_RDHALT);
	ft_putstr(prompt);
	return (rd);
}

void			read_cline(char *prompt, t_shell_config *sh)
{
	t_read rd;

	rd = init_read(prompt, sh->hist, sh->cboard, &sh->in);
	rd_config_getter(&rd);
	while (F_GET(rd.flags, F_RDHALT) && read(1, rd.b, 1))
	{
		if (rd.b[0] == ESC)
			F_SET(rd.flags, F_ESC);
		else if (TERMINAL_DELETE(rd.b[0]))
			terminal_delete_dispatcher(&rd);
		else if (F_GET(rd.flags, F_ESC) && rd.b[0] == BRA)
			F_BSET(rd.flags, F_CSI);
		else if (CUR_MOTION(rd.flags, rd.b[0]))
			cursor_move_dispatcher(&rd);
		else if (F_GET(rd.flags, F_ESC) && CBOARD_OP(rd.b[0]))
			cboard_dispatcher(&rd);
		else if (rd.b[0] == '\n')
			submit_cline(&rd);
		else if (ft_isprint(rd.b[0]))
			terminal_insert(&rd, rd.b);
	}
	sh->cboard = rd.cboard;
}
