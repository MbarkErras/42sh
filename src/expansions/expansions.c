/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 20:48:11 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/23 18:55:47 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mshell.h"

char			*get_esc_char(char *str, char c);

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

void		quotes_delimiter(char **tmp)
{
	int		i;
	int		q;
	int		dq;

	q = 0;
	dq = 0;
	i = 0;
	while ((*tmp)[i])
	{
		if (!q && (*tmp)[i] == '"' && (*tmp)[i - 1 >= 0 ? i - 1 : 0] != Q_ESCAPE
		&& (*tmp)[i - 1 >= 0 ? i - 1 : 0] != UQ_ESCAPE)
		{
			dq = !dq;
			(*tmp)[i] = D_QUOTE;
		}
		else if (!dq && (*tmp)[i] == '\'' &&
		(*tmp)[i - 1 >= 0 ? i - 1 : 0] != Q_ESCAPE &&
		(*tmp)[i - 1 >= 0 ? i - 1 : 0] != UQ_ESCAPE)
		{
			q = !q;
			(*tmp)[i] = QUOTE;
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
	*str = ft_strnew(0);
	while (*tmp)
	{
		*str = ft_fstrjoin(*str, *tmp);
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

char		*get_substring(char *str, int *k, char type)
{
	int	start;
	int	len;

	start = *k;
	len = 0;
	if (!type)
	{
		while (str[*k] && str[*k] != QUOTE && str[*k] != D_QUOTE)
		{
			(*k)++;
			len++;
		}
	}
	else
	{
		while (str[*k] && str[*k] != QUOTE)
		{
			(*k)++;
			len++;
		}
	}
	return (ft_strsub(str, start, len));
}

void		search_and_expand(char **s1, char c)
{
	char	*tmp;
	char	*dollar;
	int		i;
	int		j;
	int		k;

	j = 0;
	while ((tmp = ft_strchr((*s1) + j, c)))
	{
		k = ft_strlen(*s1) - ft_strlen(tmp);
		if (c == '$')
		{
			i = 1;
			while (ft_isalnum(tmp[i]))
				i++;
			dollar = ft_strsub(tmp, 1, i - 1);
		}
		if ((*s1)[k - 1 >= 0 ? k - 1 : 0] != UQ_ESCAPE
		&& (*s1)[k - 1 >= 0 ? k - 1 : 0] != Q_ESCAPE)
		{
			if (c == '$')
			{
				(*s1)[k] = DOLLAR;
				expand_dollar(dollar, s1, &j);
			}
			else
			{
				(*s1)[k] = TILDE;
				expand_tilde(s1, &j);
			}
		}
		else
			j = c == '$' ? k + i - 1 : j + 1;
		if (c == '$')
			free(dollar);
	}
}

void		update_arg(char *arg, char **tmp, int *k, char type)
{
	char	*s;

	if (!type || type == 1)
	{
		s = get_substring(arg, k, type);
		search_and_expand(&s, '$');
		if (!type)
			search_and_expand(&s, '~');
		else
			quoted_escape(&s);
	}
	else
	{
		(*k)++;
		s = get_substring(arg, k, 1);
		(*k)++;
	}
	*tmp = ft_fstrjoin(*tmp, s);
}

/*
** - Expand all if argument is not quoted.
** - Expand only environment variables if double quoted.
** - No expansions if argument is single quoted.
*/

void		apply_expansions(char **args)
{
	char	*s;
	char	*tmp;
	int		k;

	while (*args)
	{
		quotes_delimiter(args);
		k = 0;
		tmp = ft_strnew(0);
		while ((*args)[k])
		{
			if ((*args)[k] != QUOTE && (*args)[k] != D_QUOTE)
				update_arg(*args, &tmp, &k, 0);
			else if ((*args)[k] != QUOTE)
				update_arg(*args, &tmp, &k, 1);
			else if ((*args)[k])
				update_arg(*args, &tmp, &k, 2);
		}
		*args = tmp;
		remove_escapes(args, UQ_ESCAPE);
		remove_escapes(args, Q_ESCAPE);
		remove_quotes(args);
		args++;
	}
}
