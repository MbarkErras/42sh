/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 15:39:47 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/23 18:56:39 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

/*
** memory leaks
*/

int				errfunc(const char *epath, int errno)
{
	return (0);
}

int				get_glob_buffer(char *gl_pattern, char **args)
{
	glob_t	g;
	size_t	i;
	int		pos;
	char	*s1;
	char	*s2;

	g.gl_pathc = 0;
	glob(gl_pattern, GLOB_ERR, errfunc, &g);
	if (!g.gl_pathc)
		return (0);
	i = 0;
	while (i < g.gl_pathc)
	{
		*args = ft_strjoin(!*args ? ft_strnew(0) : *args, g.gl_pathv[i]);
		*args = ft_strjoin(*args, " ");
		i++;
	}
	globfree(&g);
	return (1);
}

char			*get_pattern(char *line, int *i, int *pos, int *start)
{
	*pos = *i;
	while (*i > 0 && line[*i] != BLANK && IS_OPERATOR(*i, _AND, _NEQ))
		(*i)--;
	*start = (*i) + 1;
	*i = *pos;
	while (*i > 0 && line[*i]
	&& line[*i] != BLANK && IS_OPERATOR(*i, _AND, _NEQ))
		(*i)++;
	return (ft_strsub(line, *start, *i - *start));
}

int				expand_wildcard(char **line, char *gl_pattern, int start, int i)
{
	char	*buf;
	char	*s1;
	char	*s2;
	int		buff_len;
	int		j;

	buf = NULL;
	j = -1;
	if (!get_glob_buffer(gl_pattern, &buf))
	{
		free(gl_pattern);
		return (0);
	}
	buff_len = ft_strlen(buf);
	while (buf[++j] && j < buff_len)
		if (buf[j] == ' ')
			buf[j] = BLANK;
	s1 = ft_strsub((*line), 0, start);
	s2 = ft_strsub((*line), i,
	ft_strlen((*line)) - ft_strlen(s1) + ft_strlen(gl_pattern) - 1);
	(*line) = ft_fstrjoin(s1, buf);
	(*line) = ft_fstrjoin((*line), s2);
	free(gl_pattern);
	return (buff_len);
}

int				update_line(char **line, int *i)
{
	int	buff_len;
	int	start;
	int	pos;

	buff_len = 0;
	start = 0;
	pos = 0;
	if (!(buff_len = expand_wildcard(line,
	get_pattern(*line, i, &pos, &start), start, *i)))
		return (0);
	*i = pos + buff_len - 1;
	return (1);
}

void			check_wildcard_c(char **line)
{
	int		i;
	char	q;
	char	dq;

	q = 0;
	dq = 0;
	i = -1;
	while ((*line)[++i])
	{
		if (!q && (*line)[i] == '"' &&
		(*line)[i - 1 >= 0 ? i - 1 : 0] != UQ_ESCAPE)
			dq = !dq;
		else if (!dq && (*line)[i] == '\'' &&
		(*line)[i - 1 >= 0 ? i - 1 : 0] != UQ_ESCAPE)
			q = !q;
		if (!q && !dq && (*line)[i] == '*' &&
		(*line)[i - 1 >= 0 ? i - 1 : 0] != UQ_ESCAPE)
		{
			if (!update_line(line, &i))
				continue ;
		}
	}
}
