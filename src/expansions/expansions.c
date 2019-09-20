/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 20:48:11 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/20 11:49:33 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mshell.h"

char	*get_esc_char(char *str, char c);

void			apply_globbing(char **line)
{
	int		i;
	int		pos;
	int		start;
	char	*buf;
	char	*gl_pattern;
	char	*s1;
	char	*s2;
	char	q;
	char	dq;

	pos = 0;
	start = 0;
	gl_pattern = NULL;
	s1 = NULL;
	s2 = NULL;
	q = 0;
	dq = 0;
	i = -1;
	while ((*line)[++i])
	{
		if (!q && (*line)[i] == '"' && (*line[i - 1] != UQ_ESCAPE))
			dq = !dq;
		else if (!dq && (*line)[i] == '\'' && (*line)[i - 1] != UQ_ESCAPE)
			q = !q;
		if (!q && !dq && (*line)[i] == '*' && (*line)[i - 1] != UQ_ESCAPE)
		{
			pos = i;
			while (i > 0 && (*line)[i] != BLANK && (*line)[i] != SEMI_COL && (*line)[i] != PIPE && (*line)[i] != OR && (*line)[i] != AND)
				i--;
			start = i + 1;
			i = pos;
			while (i > 0 && (*line)[i] && (*line)[i] != BLANK && (*line)[i] != SEMI_COL && (*line)[i] != PIPE && (*line)[i] != OR && (*line)[i] != AND)
				i++;
			gl_pattern = ft_strsub((*line), start, i - start);
			buf = NULL;
			if (!apply_glob_expansion(gl_pattern, &buf))
			{
				free(gl_pattern);
				continue ;
			}
			for (int i; i < ft_strlen(buf); i++)
				if (buf[i] == ' ')
					buf[i] = BLANK;
			s1 = ft_strsub((*line), 0, start);
			s2 = ft_strsub((*line), i, ft_strlen((*line)) - ft_strlen(s1) + ft_strlen(gl_pattern) - 1);
			(*line) = ft_fstrjoin(s1, buf);
			(*line) = ft_fstrjoin((*line), s2);
			i = pos + ft_strlen(buf) - 1;
			free(gl_pattern);
			free(buf);
		}
	}
}

char		*ft_fstrsub(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;

	if (s)
	{
		str = (char *)malloc(sizeof(*str) * (len + 1));
		if (str == NULL)
			return (NULL);
		i = 0;
		while (i < len)
		{
			str[i] = s[start + i];
			i++;
		}
		str[i] = '\0';
		free((char *)s);
		return (str);
	}
	return (NULL);
}

char		*ft_fstrjoin(char *s1, char *s2)
{
	char	*str;

	str = ft_strjoin(s1, s2);
	free((char *)s1);
	free((char *)s2);
	return (str);
}

void		quotes_delimiter(char **tmp, char *s_dq, char *s_q)
{
	int		i;
	int		q;
	int		dq;

	q = 0;
	dq = 0;
	i = 0;
	while ((*tmp)[i])
	{
		if (!q && (*tmp)[i] == '"' && (*tmp)[i - 1] != Q_ESCAPE && (*tmp)[i - 1] != UQ_ESCAPE)
		{
			dq = !dq;
			(*tmp)[i] = D_QUOTE;
			*s_dq = 1;
		}
		else if (!dq && (*tmp)[i] == '\'' && (*tmp)[i - 1] != Q_ESCAPE && (*tmp)[i - 1] != UQ_ESCAPE)
		{
			q = !q;
			(*tmp)[i] = QUOTE;
			*s_q = 1;
		}
		i++;
	}
}

void		remove_dq(char **tmp)
{
	char	**tmp2;
	char	**f;

	tmp2 = ft_strsplit(*tmp, D_QUOTE);
	f = tmp2;
	free(*tmp);
	*tmp = ft_strnew(0);
	while (*tmp2)
	{
		*tmp = ft_fstrjoin(*tmp, *tmp2);
		tmp2++;
	}
	free(f);
}

void		remove_q(char **tmp)
{
	char	**tmp2;
	char	**f;

	tmp2 = ft_strsplit(*tmp, QUOTE);
	f = tmp2;
	free(*tmp);
	*tmp = ft_strnew(0);
	while (*tmp2)
	{
		*tmp = ft_fstrjoin(*tmp, *tmp2);
		tmp2++;
	}
	free(f);
}

void		remove_quotes(char **args)
{
	int		i;
	char	*tmp;
	char	s_q;
	char	s_dq;

	i = 0;
	s_dq = 0;
	s_q = 0;
	tmp = ft_strdup(*args);
	remove_dq(&tmp);
	remove_q(&tmp);
	free(*args);
	*args = tmp;
}

void		remove_escapes(char **str, char escape)
{
	char	**tmp;

	tmp = ft_strsplit(*str, escape);
	free(*str);
	*str = NULL;
	while (*tmp)
	{
		*str = ft_strjoin((*str) ? (*str) : ft_strnew(0), *tmp);
		tmp++;
	}
}

void		expand_tilde(char **args, int *j)
{
	char	*curr;
	char	*s1;
	char	*s2;
	char	*tmp;
	char	tilde[2];

	tilde[0] = TILDE;
	tilde[1] = 0;
	s1 = ft_strsub(*args, 0, ft_strpos(*args, tilde));
	s2 = ft_strsub(*args, ft_strpos(*args, tilde) + 1,
	ft_strlen(*args) - ft_strlen(s1) - 1);
	if ((curr = getenv("HOME")))
	{
		tmp = s1;
		s1 = ft_strjoin(s1, curr);
		free(tmp);
	}
	*j = ft_strlen(s1);
	tmp = *args;
	*args = ft_fstrjoin(s1, s2);
	free(tmp);
}

void		expand_dollar(char *dollar, char **args, int *j)
{
	char	*s1;
	char	*s2;
	char	*tmp2;
	int		pos;
	char	*curr;
	char	ds[2]; 

	ds[0] = DOLLAR;
	ds[1] = 0;
	pos = ft_strpos(*args, ds);
	s1 = ft_strsub(*args, 0, pos);
	s2 = ft_strsub(*args, pos + ft_strlen(dollar) + 1,
	ft_strlen(*args) - (ft_strlen(dollar) + ft_strlen(s1)) - 1);
	if ((curr = getenv(dollar)))
	{
		tmp2 = s1;
		s1 = ft_strjoin(s1, curr);
		free(tmp2);
	}
	*j = ft_strlen(s1);
	tmp2 = *args;
	*args = ft_fstrjoin(s1, s2);
	free(tmp2);
}

void	quoted_escape(char **arg)
{
	char	*s1;
	char	*s2;
	char	*s3;
	char	*esc;
	int		pos;
	int		j;

	j = 0;
	s1 = ft_strnew(0);
	s2 = ft_strnew(0);
	while ((esc = ft_strchr((*arg) + j, Q_ESCAPE)))
	{
		pos = ft_strlen(*arg) - ft_strlen(esc);
		s1 = ft_strsub(*arg, j, pos - j);
		s1 = get_esc_char(s1, (*arg)[pos + 1]);
		s2 = ft_strjoin(s2, s1);
		j = pos + 2;
	}
	s3 = ft_strdup((*arg) + j);
	s1 = ft_fstrjoin(s2, s3);
	free(*arg);
	*arg = s1;
}

int		errfunc(const char *epath, int errno)
{
	return (0);
}

int	apply_glob_expansion(char *gl_pattern, char **args)
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

int		ft_strindex(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

void		apply_expansions(char **args)
{
	char	*tmp;
	char	*s1;
	char	*s2;
	char	*tmp2;
	char	*dollar;
	char	s_q;
	char	s_dq;
	int		k;
	int		j;
	int		i;
	char	*gl_pattern;

	s_q = 0;
	s_dq = 0;
	tmp = NULL;
	s1 = NULL;
	s2 = NULL;
	while (*args)
	{
		// printf("arg_b: %s\n", *args);
		quotes_delimiter(args, &s_dq, &s_q);
		// printf("arg_b: %s\n", *args);
		k = 0;
		tmp2 = ft_strnew(0);
		s1 = NULL;
		s2 = NULL;
		int a = 1;
		while ((*args)[k])
		{
			if ((*args)[k] != QUOTE && (*args)[k] != D_QUOTE) //expand all
			{
				int start = k;
				int len = 0;
				while ((*args)[k] && (*args)[k] != QUOTE && (*args)[k] != D_QUOTE)
				{
					k++;
					len++;
				}
				s1 = ft_strsub(*args, start, len);
				j = 0;
				while ((tmp = ft_strchr(s1 + j, '$')))
				{
					int k;
					k = ft_strlen(s1) - ft_strlen(tmp);
					i = 1;
					while (ft_isalnum(tmp[i]))
						i++;
					dollar = ft_strsub(tmp, 1, i - 1);
					if (s1[k - 1] != UQ_ESCAPE)
					{
						s1[k] = DOLLAR;
						expand_dollar(dollar, &s1, &j);
					}
					else
						j = k + i - 1;
					free(dollar);
				}
				j = 0;
				while ((tmp = ft_strchr(s1 + j, '~')))
				{
					int	k;
					k = ft_strlen(s1) - ft_strlen(tmp);
					if (s1[k - 1] != UQ_ESCAPE)
					{
						s1[k] = TILDE;
						expand_tilde(&s1, &j);
					}
					else
						j++;
				}
				remove_escapes(&s1, UQ_ESCAPE);
				tmp2 = ft_strjoin(tmp2, s1);
				s1 = NULL;
			}
			else if ((*args)[k] != QUOTE) //expand dollar only
			{
				int start = k;
				int len = 0;
				while ((*args)[k] && (*args)[k] != QUOTE)
				{
					k++;
					len++;
				}
				s1 = ft_strsub(*args, start, len);
				printf("s1: %s\n", s1);
				j = 0;
				while ((tmp = ft_strchr(s1 + j, '$')))
				{
					int k;
					k = ft_strlen(s1) - ft_strlen(tmp);
					i = 1;
					while (ft_isalnum(tmp[i]))
						i++;
					dollar = ft_strsub(tmp, 1, i - 1);
					if (s1[k - 1] != Q_ESCAPE)
					{
						s1[k] = DOLLAR;
						expand_dollar(dollar, &s1, &j);
					}
					else
						j = k + i - 1;
					free(dollar);
				}
				quoted_escape(&s1);
				remove_escapes(&s1, Q_ESCAPE);
				tmp2 = ft_strjoin(tmp2, s1);
				s1 = NULL;
			}
			else if ((*args)[k]) // no expansion
			{
				int start = k;
				int len = 0;
				k++;
				while ((*args)[k] && (*args)[k] != QUOTE)
				{
					k++;
					len++;
				}
				s2 = ft_strsub(*args, start, len + 2);
				k++;
				tmp2 = ft_strjoin(tmp2, s2);
				s2 = NULL;
			}
		}
		*args = tmp2;
		remove_quotes(args);
		// printf("arg: %s\n", *args);
		args++;
	}
}
