/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checking.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 19:50:25 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/23 22:29:40 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

/*
** Still needs testing in case it fails in valid syntax somehow.
*/

void	counter(char *line, int *j, int o)
{
	char	tmp;

	if (!o)
	{
		while (*j && IS_OPERATOR(*j, _AND, _NEQ))
			(*j)--;
		if ((IS_OPERATOR(*j, _OR, _EQ)))
			(*j)++;
	}
	else
	{
		tmp = line[*j - 1];
		while ((line[*j] && IS_OPERATOR(*j, _AND, _NEQ)) || (tmp >= OR
		&& (IS_REDIR_OP(*j, _OR, _EQ))))
			(*j)++;
	}
}

int		redir_syntax(char *line, int i)
{
	if (IS_REDIR_OP(i, _OR, _EQ))
	{
		if (line[i] != APP_OUT_RED_OP && line[i] != HEREDOC_OP
		&& ((line[i + 1] == '&' && !ft_isdigit(line[i + 2]))
		|| (line[(i - 1 > 0) ? i - 1 : 0] == '&' &&
		line[i + 1] == '&')))
			return (0);
		else if ((line[i] == APP_OUT_RED_OP || line[i] == HEREDOC_OP)
		&& line[i + 2] == '&')
			return (0);
	}
	return (1);
}

int		get_operands(char *line, int *i, char t_op, int *o_i)
{
	int		j;
	char	operands;

	operands = 0;
	*o_i = *i;
	if (!(*i) && (IS_REDIR_OP(*i, _AND, _NEQ)))
		return (operands);
	j = (*i - 1) >= 0 ? (*i - 1) : *i;
	if ((j >= 0 && IS_OPERATOR(j, _AND, _NEQ))
	|| (!j && (IS_REDIR_OP(j, _OR, _EQ))))
	{
		counter(line, &j, 0);
		if ((j != *i - 1) && is_not_blank(line, j, *i))
			operands |= LEFT_OPR;
		t_op ? (j = *i + 2)
		: (j = *i + 1);
		counter(line, &j, 1);
		if ((j != *i + 1) && redir_syntax(line, *i) &&
		is_not_blank(line, *i = t_op ? (*i + 2) : (*i + 1), j))
		{
			*i = j - 2;
			operands |= RIGHT_OPR;
		}
	}
	return (operands);
}

int		ft_putendline(char const *s)
{
	int i;

	i = 0;
	while (s[i] != '\0')
	{
		ft_putchar(s[i]);
		i++;
	}
	ft_putchar('\n');
	return (1);
}

int		check_syntax_errors(char *line)
{
	int		i;
	char	t_op;
	char	ops;
	int		j;

	i = -1;
	while (line[++i])
	{
		t_op = 0;
		if (IS_OPERATOR(i, _OR, _EQ))
		{
			if (TWO_C_OPS(i, _OR, _EQ))
				t_op = 1;
			ops = get_operands(line, &i, t_op, &j);
			if (((ops & LEFT_OPR) == LEFT_OPR && (line[j] == SEMI_COL ||
			line[j] == BG))
			|| ((IS_REDIR_OP(j, _OR, _EQ)) && (ops & RIGHT_OPR) == RIGHT_OPR))
				continue ;
			else if ((ops & RIGHT_OPR) != RIGHT_OPR
			|| (ops & LEFT_OPR) != LEFT_OPR)
				return (ft_putendline("syntax error."));
		}
	}
	return (0);
}
