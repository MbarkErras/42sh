/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 16:45:13 by merras            #+#    #+#             */
/*   Updated: 2019/09/22 22:11:58 by mmostafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

int		b_echo(char **arg)
{
	int i;
	int	new_line;

	new_line = 0;
	i = 0;
	while (arg[++i] && arg[i][0] == '-' && is_flag(arg[i] + 1, "n"))
		new_line = 1;
	while (arg[i])
	{
		ft_putstr(arg[i]);
		if (arg[i + 1])
			ft_putchar(' ');
		i++;
	}
	if (!new_line)
		ft_putchar('\n');
	else
		ft_putstr("%\n");
	return (1);
}

int		b_senv(char **in, int p)
{
	int i;

	i = 0;
	if (p)
	{
		if (!in[1])
			print_array(env_converter());
		else if (array_length(in) > 3)
			ft_perror("setenv", NULL, X_ARG);
		else if (!isalnum_check(in[1]) || !ft_strlen(in[1]))
			ft_perror("setenv", NULL, N_CHN);
		else
			update_env(in[1], in[2] ? in[2] : "", 1);
	}
	else
	{
		if (!in[1])
			return (ft_perror("unsetenv", NULL, N_ARG));
		while (in[++i])
			update_env(in[i], NULL, 0);
	}
	return (0);
}

/*
t_list	*list_copy(t_list *origin)
{
	t_list	*copy;
	t_list	*head;
	t_list	*head2;

	copy = malloc(sizeof(t_list));
	((t_variable *)copy->content)->value = ft_strdup(((t_variable *)origin->content)->value);
	((t_variable *)copy->content)->flag = ((t_variable *)origin->content)->flag;
	copy->next = NULL;
	head = copy;
	head2 = head;
	origin = origin->next;
	while (origin)
	{		
		copy = malloc(sizeof(t_list));
		((t_variable *)copy->content)->value = ft_strdup(((t_variable *)origin->content)->value);
		((t_variable *)copy->content)->flag = ((t_variable *)origin->content)->flag;
		copy->next = NULL;
		head2->next = copy;
		head2 = head2->next;
		origin = origin->next;
	}
	head2 = head;
	while (head2)
	{
		printf("%s\n",((t_variable *)copy->content)->value);
		head2 = head2->next;
	}
	return (head);
}
*/

t_list	*list_copy(t_list *list)
{
	t_list *copy;
	t_list *i;

	if (!list)
		return (NULL);
	copy = NULL;
	i = NULL;
	while (list)
	{
		if (!i)
		{
			i = (t_list *)malloc(sizeof(t_list));
			i->content = create_variable(((t_variable *)list->content)->value, ((t_variable *)list->content)->flag);
			copy = i;
		}
		else
		{
			i->next = (t_list *)malloc(sizeof(t_list));
			i->next->content = create_variable(((t_variable *)list->content)->value, ((t_variable *)list->content)->flag);
			i = i->next;
		}
		list = list->next;
	}
	return (copy);
}

void	b_env_arg(char **in)
{
	int			i;
	char		**env_input;
	t_list		*save;
	t_list		*temp;

	temp = list_copy(sh_config_getter(NULL)->variables);
	save = sh_config_getter(NULL)->variables;
	sh_config_getter(NULL)->variables = temp;
	i = b_env_updater(in);
	env_input = in + i;
	if (in[i])
	{
		//execute_command(&env_input);
		F_SET(sh_config_getter(NULL)->flags, 20);
	}
	if (!F_GET(sh_config_getter(NULL)->flags, 20))
		print_array(env_converter());
	list_delete(temp, hist_node_del);
	sh_config_getter(NULL)->variables = save;
	F_UNSET(sh_config_getter(NULL)->flags, 20);
}

int		b_env(char **in)
{
	if (!in[1])
	{
		print_array(env_converter());
		return (0);
	}
	b_env_arg(in);
	return (1);
}

void	b_exit(char **input)
{
	exit(input[1] ? ft_atoi(input[1]) : 0);
}
