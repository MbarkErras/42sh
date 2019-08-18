/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape_god.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/14 15:23:46 by aariss            #+#    #+#             */
/*   Updated: 2019/08/17 15:41:09 by yoyassin         ###   ########.fr       */
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
	else
		str = ft_joinchar(str, c);
	return (str);
}
