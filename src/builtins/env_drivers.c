/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_drivers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 18:36:04 by merras            #+#    #+#             */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	update_env_value(char *entry, t_list *prev, t_list *en)
{
	t_list *new;

	new = list_create_node(create_variable(entry, 1),
			sizeof(t_variable));
	if (prev)
		prev->next = new;
	else
		prev = new;
	delete_variable(en->content);
	free(en);
}

void	update_env(char *key, char *value, int flag)
{
	t_list		*temp;
	t_list		*en;

	en = sh_config_getter(NULL)->variables;
	temp = NULL;
	while (en)
	{
		if (((t_variable *)en->content)->flag &&
			ft_strstr(((t_variable *)en->content)->value, key) == ((t_variable *)en->content)->value &&
			((t_variable *)en->content)->value[ft_strlen(key)] == '=')
		{
			if (flag)
			{
				update_env_value(
					ft_strjoin_free(ft_strjoin(key, "="), value, 1), temp, en);
			}
			else
				list_delete_node(temp, &en, delete_variable);
			return ;
		}
		temp = en;
		en = en->next;
	}
	if (flag)
		list_push_back(&sh_config_getter(NULL)->variables,
				list_create_node(create_variable(ft_strjoin_free(ft_strjoin(key, "="), value, 1), 1), sizeof(t_variable)));
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
