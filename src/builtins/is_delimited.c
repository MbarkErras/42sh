/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_delimited.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/02 17:53:39 by merras            #+#    #+#             */
/*   Updated: 2019/08/02 18:30:51 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

int		is_delimited(char *string, int c, char d)
{
	int i;
	int	d_toggle;

	d_toggle = 0;
	i = 0;
	while (string[i])
	{
		if (string[i] == d)
			d_toggle = !d_toggle;
		if (i == c)
			return (d_toggle);
		i++;
	}
	return (0);
}

void	character_delete(char **string, char to_delete)
{
	int i;

	i = -1;
	while ((*string)[++i])
	{
		if ((*string)[i] == to_delete && to_delete == '\'' &&
				is_delimited(*string, i, '"'))
			continue ;
		if ((*string)[i] == to_delete)
		{
			*string = delete_chars((*string), i, 1);
			i--;
		}
	}
}

void	character_replace(char *string, char find, char replace)
{
	while (*string)
	{
		if (*string == find)
			*string = replace;
		string++;
	}
}

char	*parameter_replace(char *raw, int dollar_pos)
{
	char	*expanded;
	char	*key;
	int		j;

	if (is_delimited(raw, dollar_pos, '\''))
		return (NULL);
	expanded = ft_strsub(raw, 0, dollar_pos);
	j = dollar_pos + 1;
	while (raw[j] && ft_isalnum(raw[j]))
		j++;
	key = ft_strsub(raw, dollar_pos + 1, j - dollar_pos - 1);
	expanded = ft_strjoin_free(expanded, read_env(key), 1);
	expanded =
		ft_strjoin_free(expanded, raw + dollar_pos + ft_strlen(key) + 1, 1);
	free(key);
	return (expanded);
}
