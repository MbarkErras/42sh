/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 15:56:54 by merras            #+#    #+#             */
/*   Updated: 2019/09/24 12:02:37 by merras           ###   ########.fr       */
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

t_read			init_read(char *prompt, t_list *hist, char *cboard, char **in)
{
	t_read		rd;
	t_list		*tail;

	tail = hist;
	while (tail && tail->next)
		tail = tail->next;
	rd.hist = tail;
	rd.editing_history_node = NULL;
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
	sh->rd = init_read(prompt, sh->hist, sh->cboard, &sh->in);
	rd_config_getter(&sh->rd);
	while (F_GET(sh->rd.flags, F_RDHALT) && read(1, sh->rd.b, 1))
	{
		if (sh->rd.b[0] == CTRL_R)
			reverse_i_search(&sh->rd);
		if (sh->rd.b[0] == ESC)
			F_SET(sh->rd.flags, F_ESC);
		else if (TERMINAL_DELETE(sh->rd.b[0]))
			terminal_delete_dispatcher(&sh->rd);
		else if (F_GET(sh->rd.flags, F_ESC) && sh->rd.b[0] == BRA)
			F_BSET(sh->rd.flags, F_CSI);
		else if (CUR_MOTION(sh->rd.flags, sh->rd.b[0]))
			cursor_move_dispatcher(&sh->rd);
		else if (F_GET(sh->rd.flags, F_ESC) && CBOARD_OP(sh->rd.b[0]))
			cboard_dispatcher(&sh->rd);
		else if (sh->rd.b[0] == '\n')
			submit_cline(&sh->rd);
		else if (ft_isprint(sh->rd.b[0]))
			terminal_insert(&sh->rd, sh->rd.b);
		ack_history_change(sh->rd.editing_history_node);
	}
	sh->cboard = sh->rd.cboard;
}
