/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 21:33:00 by merras            #+#    #+#             */
/*   Updated: 2019/09/23 23:49:34 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

int	ft_export(char **arg)
{
	t_list	*variables;	
	variables = sh_config_getter(NULL)->variables;
	arg++;
	if (!(*arg))
		return (b_env(arg));
	while (*arg)
	{
		variables = sh_config_getter(NULL)->variables;
		while (variables)
		{
			if (ft_strstr(((t_variable *)variables->content)->value, *arg) &&
						((t_variable *)variables->content)->value[ft_strlen(*arg) == '='])
			{
				if (((t_variable *)variables->content)->flag == 0)
					((t_variable *)variables->content)->flag = 1;
				break;
			}
		}
		if (!variables)
			b_senv(arg, 0);
		arg++;
	}
	return (0);
}

int ft_set()
{
	t_list *variables;

	variables = sh_config_getter(NULL)->variables;
	while (variables)
	{
		ft_putendl(((t_variable *)variables->content)->value);
		variables = variables->next;
	}
	return (0);
}

int	ft_unset(char **arg)
{
	t_list *variables;

	if (!arg)
		return (1);
	variables = sh_config_getter(NULL)->variables;
	arg++;
	while (*arg)
	{
		variables = sh_config_getter(NULL)->variables;
		while (variables)
		{
			// if (ft_strstr(((t_variable *)variables->content)->value, *arg) &&
			// 		((t_variable *)variables->content)->value[ft_strlen(*arg) == '='])
			// 	list_delete_node(sh_config_getter(NULL)->variables,
			// 			&variables,
			// 			delete_variable);
			variables = variables->next;
		}
		arg++;
	}
	return (0);
}
