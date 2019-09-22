/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_drivers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 18:36:04 by merras            #+#    #+#             */
/*   Updated: 2019/09/22 15:57:58 by mmostafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	update_env_value(char *entry, t_string *prev, t_string *en)
{
	t_string *new;

	new = t_string_create_node((t_string){entry, en->next, NULL});
	if (prev)
		prev->next = new;
	else
		prev = new;
	free(en->string);
	free(en);
}

void	delete_env_value(t_string *prev, t_string *en)
{
	if (!(en == sh_config_getter(NULL)->variables))
	{
		if (en->next)
			prev->next = en->next;
		else
			prev->next = NULL;
	}
	free(en);
	free(en->string);
}

void	add_env_value(char *entry, t_string *prev)
{
	t_string *added;

	added = t_string_create_node((t_string){entry, NULL, NULL});
	if (prev)
		prev->next = added;
	else
		prev = added;
}

void	update_env(char *key, char *value, int flag)
{
	t_list		*temp;
	t_list		*en;

	en = sh_config_getter(NULL)->variables;
	temp = NULL;
	while (en)
	{
		if (ft_strstr(en->string, key) == en->string &&
				en->string[ft_strlen(key)] == '=')
		{
			if (flag)
			{
				update_env_value(
					ft_strjoin_free(ft_strjoin(key, "="), value, 1), temp, en);
			}
			else
				delete_env_value(temp, en);
			return ;
		}
		temp = en;
		en = en->next;
	}
	if (flag)
		add_env_value(ft_strjoin_free(ft_strjoin(key, "="), value, 1), temp);
}

char	*read_env(char *key)
{
	char	**env;
	char	*value;

	printf("HNA !!\n");
	env = env_converter();
	printf("MACHI HNA !!\n");
	if (!env || !key)
		return (NULL);
	while (*env)
	{
		if ((value = ft_strstr(*env, key)) && value == *env &&
				value[ft_strlen(key)] == '=')
			return (value + ft_strlen(key) + 1);
		env++;
	}
	return (NULL);
}
