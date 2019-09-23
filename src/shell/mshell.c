/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

t_shell_config	*sh_config_getter(t_shell_config *sh_set)
{
	static t_shell_config *sh_get = NULL;

	if (sh_set)
		sh_get = sh_set;
	return (sh_get);
}

int				env_variables_counter(t_list *v)
{
	int i;

	i = 0;
	while (v)
	{
		if (((t_variable *)v->content)->flag)
			i++;
		v = v->next;
	}
	return (i);
}

char			**env_converter(void)
{
	static char **array = NULL;
	int			i;
	t_list		*list;

	list = sh_config_getter(NULL)->variables;
	if (!list)
		return (NULL);
	if (array)
		free(array);
	if (!(array = (char **)malloc(sizeof(char *) *
					(env_variables_counter(list) + 1))))
		exit_cleanup(EXIT_FAILURE, F_EXE);
	i = 0;
	while (list)
	{
		if (((t_variable *)list->content)->flag)
		{
			array[i] = ((t_variable *)list->content)->value;
			i++;
		}
		list = list->next;

	}
	array[i] = NULL;
	return (array);
}

void			signal_handler(int sig)
{
	t_shell_config	*sh;
	t_read			*rd;

	(void)sig;
	sh = sh_config_getter(NULL);
	ft_putchar('\n');
	if (F_GET(sh->flags, F_CHILDPROC))
		return ;
	rd = rd_config_getter(NULL);
	sh->rd = init_read(PRMPT(F_GET(sh->flags, F_LASTRET)),
	sh->hist, sh->cboard, &(sh->in));
}

void			save_io(void)
{
	dup2(0, STDIN_DUP);
	dup2(1, STDOUT_DUP);
	dup2(2, STDERR_DUP);
}

t_variable	*create_variable(char *value, int flag)
{
	t_variable *variable;

	if (!value)
		return (NULL);
	if (!(variable = malloc(sizeof(t_variable))))
		return (NULL);
	variable->value = value;
	variable->flag = flag;
	return (variable);
}

t_list	*array_to_list(char **array)
{
	t_list	*local_variables;
	t_variable *variable;

	local_variables = NULL;
	if (!array)
		return ((t_list *)-1);
	while (*array)
	{
		if (!(variable = create_variable(ft_strdup(*array), 1)))
			return ((t_list *)-1);
		list_push_back(&local_variables,
					list_create_node(variable, sizeof(t_variable)));
		array++;
	}
	return (local_variables);
}

void	delete_variable(void *variable)
{
	t_variable *v;

	if (!variable)
		return ;
	v = (t_variable *)variable;
	free(v->value);
	free(v);
}

static void		init_shell_config(t_shell_config *sh)
{
	extern char	**environ;

	if ((int)(sh->variables = array_to_list(environ)) == -1)
		exit_cleanup(EXIT_FAILURE, F_EXE);
	sh->hist = NULL;
	sh->cboard = ft_strnew(0);
	sh->flags = 1;
	signal(SIGINT, signal_handler);
	sh_config_getter(sh);
	init_terminal();
	save_io();
}

int				main(void)
{
	t_shell_config sh;

	init_shell_config(&sh);
	while (1)
	{
		read_cline(PRMPT(F_GET(sh.flags, F_LASTRET)), &sh);
		if (ft_strlen(*rd_config_getter(NULL)->in))
		{
			list_push_back(&sh.hist, list_create_node(t_hist_construct((t_hist){ft_strdup(*rd_config_getter(NULL)->in), time(NULL), NULL, 2}), sizeof(t_hist)));
			//read_history_resetting();
		}
		printf("result: %s\n", result_giving(sh.in, sh.hist));
		if (ft_strequ(sh.in, "exit"))
		{
			system("reset");
			exit(0);
		}
		// char **temp = malloc(sizeof(char **) * 2);
		// temp[1] = NULL;
		// b_history(temp);
		//if (!(sh.jobs = parse(&sh)))
		//	continue ;
	}
}
