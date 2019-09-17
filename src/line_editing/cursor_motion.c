/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 21:00:04 by merras            #+#    #+#             */
/*   Updated: 2019/08/17 17:56:19 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	end_home(t_read *rd)
{
	if (rd->b[0] == END)
	{
		while (rd->pos < (int)(rd->prmpt + ft_strlen(*(rd->in))))
			move_right(rd);
	}
	if (rd->b[0] == HOME)
	{
		while (rd->pos > rd->prmpt)
			move_left(rd);
	}
}

void	move_right(t_read *rd)
{
	if (F_GET(rd->flags, F_SMODE))
	{
		tputs(tgetstr("us", NULL), 1, ft_putchar_int);
		ft_putchar((*(rd->in))[rd->pos - rd->prmpt]);
		tputs(tgetstr("ue", NULL), 1, ft_putchar_int);
	}
	else
	{
		tputs(tgetstr("nd", NULL), 1, ft_putchar_int);
		if ((*(rd->in))[rd->pos - rd->prmpt] == '\n' ||
				rd->c % rd->term.ws_col == rd->term.ws_col - 1)
		{
			tputs(tgetstr("do", NULL), 0, ft_putchar_int);
			rd->r++;
			rd->c = -1;
		}
	}
	rd->c++;
	rd->pos++;
}

void	move_left(t_read *rd)
{
	if (F_GET(rd->flags, F_SMODE) && !(rd->pos - rd->prmpt > rd->cb_start))
		return ;
	if (F_GET(rd->flags, F_SMODE))
	{
		tputs(tgetstr("le", NULL), 1, ft_putchar_int);
		ft_putchar((*(rd->in))[rd->pos - rd->prmpt - 1]);
	}
	if (!rd->c && rd->r)
	{
		tputs(tgetstr("up", NULL), 0, ft_putchar_int);
		rd->r--;
		if ((*(rd->in))[rd->pos - rd->prmpt - 1] == '\n')
			move_left_wrap(rd);
		else
		{
			tputs(tparm(tgetstr("RI", NULL), rd->term.ws_col), 1,
					ft_putchar_int);
			rd->c = rd->term.ws_col;
		}
	}
	else
		tputs(tgetstr("le", NULL), 1, ft_putchar_int);
	rd->c--;
	rd->pos--;
}

/*
void	navigate_hist(t_read *rd)
{
	if (F_GET(rd->flags, F_HIST) && (rd->b[0] == 'A' ? rd->hist : rd->hist))
	{
		if (rd->b[0] == 'B' && !rd->hist->next)
		{
			get_left(rd);
			tputs(tgetstr("cd", NULL), 1, ft_putchar_int);
			free_and_init(rd->in);
			terminal_insert(rd, "");
			return ;
		}
		if (rd->b[0] == 'A' && !rd->hist->prev)
			return ;
		rd->hist = rd->b[0] == 'A' ? rd->hist->prev : rd->hist->next;
	}
	if (rd->hist)
	{
		if (rd->b[0] == 'B' && !F_GET(rd->flags, F_HIST))
			return ;
		get_left(rd);
		tputs(tgetstr("cd", NULL), 1, ft_putchar_int);
		free_and_init(rd->in);
		terminal_insert(rd, rd->hist->string);
	}
	F_SET(rd->flags, F_HIST);
}
*/

void	navigate_hist(t_read *rd)
{
	if (F_GET(rd->flags, F_HIST) && rd->hist)
	{
		if (rd->b[0] == 'A' && !rd->hist->prev)
			return ;
		rd->hist = rd->b[0] == 'A' ? rd->hist->prev : rd->hist->next;
	}
	if (rd->hist)
	{
		get_left(rd);
		tputs(tgetstr("cd", NULL), 1, ft_putchar_int);
		rd->in = &((t_hist *)(rd->hist->content))->value;	
		rd->editing_history_node = rd->hist;
		terminal_insert(rd, "");
		get_right(rd);
	}
	F_SET(rd->flags, F_HIST);
}

void	move_character(t_read *rd)
{
	if ((rd->b[0] == 'A' || rd->b[0] == 'B') && rd->hist)
	{
		if (rd->b[0] == 'B' && !rd->hist->next)
		{
				get_left(rd);
				tputs(tgetstr("cd", NULL), 1, ft_putchar_int);
				rd->in = &sh_config_getter(NULL)->in;	
				rd->editing_history_node = NULL;
				terminal_insert(rd, "");
				get_right(rd);
				F_UNSET(rd->flags, F_HIST);
				return ;
		}
		navigate_hist(rd);
	}
	if (rd->b[0] == 'C' && rd->pos < rd->prmpt + (int)ft_strlen(*(rd->in)))
		move_right(rd);
	if (rd->b[0] == 'D' && rd->pos > rd->prmpt)
		move_left(rd);
	F_BUNSET(rd->flags, 3);
}
