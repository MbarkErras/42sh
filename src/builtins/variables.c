/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 21:33:00 by merras            #+#    #+#             */
/*   Updated: 2019/09/21 16:11:00 by mmostafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

int	ft_export(char **arg)
{
	
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
			if (ft_strstr(((t_variable *)variables->content)->value, *arg) &&
					((t_variable *)variables->content)->value[ft_strlen(*arg) == '='])
				list_delete_node(sh_config_getter(NULL)->variables,
						&variables,
						delete_variable);
			variables = variables->next;
		}
		arg++;
	}
	return (0);
}
