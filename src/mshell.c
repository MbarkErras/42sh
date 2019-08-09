/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/07 04:38:45 by merras            #+#    #+#             */
/*   Updated: 2019/08/09 20:48:04 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 21:48:55 by merras            #+#    #+#             */
/*   Updated: 2019/08/07 04:18:55 by merras           ###   ########.fr       */
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

char			**env_converter(void)
{
	static char **array = NULL;
	int			i;
	t_string	*list;

	list = sh_config_getter(NULL)->env;
	if (!list)
		return (NULL);
	if (array)
		free(array);
	if (!(array = (char **)malloc(sizeof(char *) *
					(t_string_length(list) + 1))))
		exit_cleanup(EXIT_FAILURE, F_EXE);
	i = 0;
	while (list)
	{
		array[i] = list->string;
		list = list->next;
		i++;
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
	*rd = init_read(PRMPT(F_GET(sh->flags, F_LASTRET)),
			sh->hist, sh->cboard, &(sh->in));
}

void			save_io(void)
{
	dup2(0, STDIN_DUP);
	dup2(1, STDOUT_DUP);
	dup2(2, STDERR_DUP);
}

static void		init_shell_config(t_shell_config *sh)
{
	extern char	**environ;

	if ((int)(sh->env = array_to_t_string(environ)) == -1)
		exit_cleanup(EXIT_FAILURE, F_EXE);
	sh->hist = NULL;
	sh->cboard = ft_strnew(0);
	sh->flags = 1;
	signal(SIGINT, signal_handler);
	sh_config_getter(sh);
	init_terminal(sh);
	save_io();
}

int				main(void)
{
	t_shell_config sh;

	init_shell_config(&sh);
	while (1)
	{
		//check_jobs_state();
		read_cline(PRMPT(F_GET(sh.flags, F_LASTRET)), &sh);
		if (ft_strlen(sh.in))
			t_string_push(&sh.hist, sh.in);
		if (ft_strequ(sh.in, "exit"))
			exit(0);
		if (!(sh.jobs = parse(&sh)))
			continue ;
		t_string_push(&sh.hist, sh.in);
		// execute_jobs(sh.jobs);
		// /!\ execution cleanup function required
	}
}
