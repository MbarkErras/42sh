/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_line_editor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/25 08:03:48 by merras            #+#    #+#             */
/*   Updated: 2019/08/17 13:01:59 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

char	*delete_chars(char *str, int start, int size)
{
	char	*deleted;

	deleted = ft_strsub(str, 0, start);
	deleted = ft_strjoin_free(deleted, str + start + size, 1);
	free(str);
	return (deleted);
}

char	*insert_chars(char *input, char *insert, int pos)
{
	char	*inserted;

	inserted = ft_strsub(input, 0, pos);
	inserted = ft_strjoin_free(inserted, insert, 1);
	inserted = ft_strjoin_free(inserted, input + pos, 1);
	ft_strdel(&input);
	return (inserted);
}

void	terminal_delete(t_read *rd, int length)
{
	int i;
	int temp;

	i = 0;
	temp = rd->pos;
	while (i < length)
	{
		move_left(rd);
		i++;
	}
	rd->pos = temp;
	*(rd->in) = delete_chars(*(rd->in), rd->pos - rd->prmpt - length, length);
	tputs(tgetstr("cd", NULL), 1, ft_putchar_int);
	tputs(tgetstr("sc", NULL), 1, ft_putchar_int);
	i = rd->pos - rd->prmpt - length - 1;
	while ((*(rd->in))[++i])
		ft_putchar((*(rd->in))[i]);
	tputs(tgetstr("rc", NULL), 1, ft_putchar_int);
	rd->pos -= length;
}

void	terminal_delete_forward(t_read *rd)
{
	int i;

	if (!ft_strlen(*(rd->in)))
		exit_cleanup(EXIT_SUCCESS, -1);
	tputs(tgetstr("cd", NULL), 1, ft_putchar_int);
	*(rd->in) = delete_chars(*(rd->in), rd->pos - rd->prmpt, 1);
	tputs(tgetstr("sc", NULL), 1, ft_putchar_int);
	i = rd->pos - rd->prmpt - 1;
	while ((*(rd->in))[++i])
		ft_putchar((*(rd->in))[i]);
	tputs(tgetstr("rc", NULL), 1, ft_putchar_int);
}

void	terminal_insert(t_read *rd, char *insert)
{
	int i;

	rd->flags &= ~1;
	tputs(tgetstr("cd", NULL), 1, ft_putchar_int);
	*(rd->in) = insert_chars(*(rd->in), insert, rd->pos - rd->prmpt);
	i = rd->pos - rd->prmpt - 1;
	tputs(tgetstr("sc", NULL), 1, ft_putchar_int);
	while ((*(rd->in))[++i])
		ft_putchar((*(rd->in))[i]);
	tputs(tgetstr("rc", NULL), 1, ft_putchar_int);
	i = -1;
	while (insert[++i])
		move_right(rd);
}
