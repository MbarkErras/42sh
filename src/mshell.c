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
	sh->rd = init_read(/*PRMPT(F_GET(sh->flags, F_LASTRET))*/"s> ",
			sh->hist, sh->cboard, &(sh->in));
}

void			save_io(void)
{
	dup2(0, STDIN_DUP);
	dup2(1, STDOUT_DUP);
	dup2(2, STDERR_DUP);
}

void	assert_foreground(t_shell_config *sh)
{
	while (tcgetpgrp (0) != (sh->shell_pgid = getpgrp ()))
		kill (- sh->shell_pgid, SIGTTIN);
}

static void		init_shell_config(t_shell_config *sh)
{
	extern char	**environ;

	assert_foreground(sh);
	signal (SIGINT, SIG_IGN);
	//signal (SIGQUIT, SIG_IGN);
	signal (SIGTSTP, SIG_IGN);
	signal (SIGTTIN, SIG_IGN);
	signal (SIGTTOU, SIG_IGN);
	signal (SIGCHLD, SIG_IGN);
	sh->shell_pgid = getpid();
	if (setpgid(sh->shell_pgid, sh->shell_pgid) < 0)
	{
		perror ("Couldn't put the shell in its own process group");
		exit (1);
	}
	tcsetpgrp (0, sh->shell_pgid);
	if ((int)(sh->env = array_to_t_string(environ)) == -1)
		exit_cleanup(EXIT_FAILURE, F_EXE);
	sh->hist = NULL;
	F_SET(sh->flags, F_INTERACTIVE);
	sh->cboard = ft_strnew(0);
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
		read_cline("r> "/*PRMPT(F_GET(sh.flags, F_LASTRET))*/, &sh);
		if (ft_strlen(*rd_config_getter(NULL)->in))
		{
			list_push_back(&sh.hist, list_create_node(t_hist_construct((t_hist){ft_strdup(*rd_config_getter(NULL)->in), time(NULL), NULL, 2}), sizeof(t_hist)));
			read_history_resetting();
		}
		if (!(sh.jobs = parse(&sh)))
		{
			printf("null parsing\n");
		}
		execute_jobs(sh.jobs);
		//
		t_job *job;
		job = sh.jobs;
		while (job)
		{
			if (!job_is_stopped(job->processes))
					monitor_job(job);
			job = job->next; 
		}
		//
	}
}
