/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/26 16:45:13 by merras            #+#    #+#             */
/*   Updated: 2019/08/04 00:13:30 by merras           ###   ########.fr       */
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

void	b_env_arg(char **in)
{
	int			i;
	char		**env_input;
	t_string	*save;
	t_string	*temp;

	temp = t_string_copy(sh_config_getter(NULL)->env);
	save = sh_config_getter(NULL)->env;
	sh_config_getter(NULL)->env = temp;
	i = b_env_updater(in);
	env_input = in + i;
	if (in[i])
	{
		//execute_command(&env_input);
		F_SET(sh_config_getter(NULL)->flags, 20);
	}
	if (!F_GET(sh_config_getter(NULL)->flags, 20))
		print_array(env_converter());
	t_string_del(temp);
	sh_config_getter(NULL)->env = save;
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
