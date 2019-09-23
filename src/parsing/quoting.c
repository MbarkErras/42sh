/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/12 17:04:55 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/23 22:52:45 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mshell.h"

int		ending_dquote(char *line, int *i, int *j)
{
	char	*endq;

	while ((endq = ft_strchr(line + *i + 1, '"')))
	{
		*i = ft_strlen(line) - ft_strlen(endq);
		while (*j < *i)
		{
			if (line[*j] == 92 && line[*j - 1] != Q_ESCAPE)
				line[*j] = Q_ESCAPE;
			(*j)++;
		}
		if (line[*i - 1] != Q_ESCAPE)
			return (1);
	}
	return (0);
}

void		get_input(char **line, t_shell_config *sh, char *p)
{
	char	*tmp;

	tmp = NULL;
	*line = ft_fstrjoin(*line, ft_strdup("\n"));
	read_cline(p, sh);
	tmp = *line;
	*line = ft_strjoin(*line, *(sh->rd.in));
	free(tmp);
}

int			ending_quote(char *line, int *i)
{
	char	*endq;

	if ((endq = ft_strchr(line + *i + 1, '\'')))
	{
		*i = ft_strlen(line) - ft_strlen(endq);
		return (1);
	}
	return (0);
}

void		dquotes_checker(char **line, char *dq, int *i, int *j)
{
	*dq = !(*dq);
	if (ending_dquote(*line, i, j))
		*dq = !(*dq);
	while (*dq)
	{
		get_input(line, sh_config_getter(NULL), "dq> ");
		if (ending_dquote(*line, i, j))
			*dq = !(*dq);
	}
}

void		squotes_checker(char **line, char *q, int *i)
{
	*q = !(*q);
	if (ending_quote(*line, i))
		*q = !(*q);
	while (*q)
	{
		get_input(line, sh_config_getter(NULL), "q> ");
		if (ending_quote(*line, i))
			*q = !(*q);
	}
}
