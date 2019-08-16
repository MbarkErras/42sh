/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape_god.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 15:23:46 by aariss            #+#    #+#             */
/*   Updated: 2019/08/15 10:56:05 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mshell.h"

char	*container(char *s, char c)
{
	char	*str;
	int		s_size;
	int		i;

	s_size = ft_strlen(s);
	if (!(str = (char*)malloc(sizeof(char) * (s_size + 2))))
		return (0);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = c;
	str[i + 1] = '\0';
	return (str);
}

char	*ft_joinchar(char *s, char c)
{
	char *tmp;

	tmp = s;
	s = container(s, c);
	ft_strdel(&tmp);
	return (s);
}

int		ft_isquote(int c)
{
	if (c == 39 || c == '"')
		return (1);
	return (0);
}

void	escape_out_quote(char *line, char **str, int *i)
{
	(*i)++;
	if (line[*i] == '"')
	{
		*str = ft_joinchar(*str, 39);
		*str = ft_joinchar(*str, line[*i]);
		*str = ft_joinchar(*str, 39);
	}
	else if (line[*i] == 39)
	{
		*str = ft_joinchar(*str, '"');
		*str = ft_joinchar(*str, line[*i]);
		*str = ft_joinchar(*str, '"');
	}
	else
		*str = ft_joinchar(*str, line[*i]);
}

char	*get_esc_char(char *str, char c)
{
	if (c == 'n')
		str = ft_joinchar(str, '\n');
	else if (c == 't')
		str = ft_joinchar(str, '\t');
	else if (c == 'r')
		str = ft_joinchar(str, '\r');
	else if (c == 'v')
		str = ft_joinchar(str, '\v');
	else if (c == 'f')
		str = ft_joinchar(str, '\f');
	// else if (c == keeper)
	// {
	// 	str = ft_joinchar(str, -7);
	// 	str = ft_joinchar(str, c);
	// 	str = ft_joinchar(str, -7);
	// }
	else
		str = ft_joinchar(str, c);
	return (str);
}

// char	*escape_focker(char *line)
// {
// 	int		i;
// 	int		keeper;
// 	char	*str;

// 	str = ft_strdup("");
// 	i = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == UQ_ESCAPE)
// 			escape_out_quote(line, &str, &i);
// 		else if (ft_isquote(line[i]))
// 		{
// 			keeper = line[i];
// 			i++;
// 			while (line[i] != keeper)
// 			{
// 				if (line[i] == 92)
// 					str = escape_in_quote(str, line, &i, keeper);
// 				i++;
// 			}
// 		}
// 		else
// 			str = ft_joinchar(str, line[i]);
// 		i++;
// 	}
// 	return (str);
// }
