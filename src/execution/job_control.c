/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 21:25:46 by merras            #+#    #+#             */
/*   Updated: 2019/09/24 12:34:47 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	monitor_job(t_job *j)
{
	t_job *jobs;
	t_job *m;

	jobs = sh_config_getter(NULL)->jobs;
	m =  sh_config_getter(NULL)->monitored;
	if (!jobs || !j || !m)
	{
		if (!m)
			sh_config_getter(NULL)->monitored = j;
		return ;
	}
	if (jobs == j)
		sh_config_getter(NULL)->jobs = j->next;
	while (m->next)
		m = m->next;
	m->next = j;
	while (jobs->next)
	{
		if (jobs->next == j)
			jobs->next = jobs->next->next;
		jobs = jobs->next;
	}
}

int	jobslist_size(t_job *l)
{
	int i;

	i = 0;
	while (l)
	{
		l = l->next;
		i++;
	}
	return (i);
}

int	parse_jobspec(char *jspec)
{
	int spec;
	t_job *monitored;

	if (!jspec)
		return (jobslist_size(sh_config_getter(NULL)->monitored));
	if (jspec[0] != '%' || ft_strlen(jspec) < 2)
		return (0);
	if (jspec[1] == '%' || jspec[1] == '+')
		return (jobslist_size(sh_config_getter(NULL)->monitored));
	if (jspec[2] == '-')
		return (jobslist_size(sh_config_getter(NULL)->monitored) - 1);
	if (ft_digitscount((spec = ft_atoi(jspec + 1))) == ft_strlen(jspec + 1))
		return (ft_atoi(jspec + 1));
	monitored = sh_config_getter(NULL)->monitored;
	spec = 1;
	while (monitored)
	{
		if (!ft_strncmp(monitored->command, jspec + 1, ft_strlen(jspec + 1)))
			return (spec);
		spec++;
	}
	return(0);
}

int	jobcontrol_ground_manager(char **arg)
{
	pid_t pid;

	if (arg)
		return (0);
	while (*arg)
	{
		if (!(pid = parse_jobspec(*arg)))
			return (1);
		if (pid > jobslist_size(sh_config_getter(NULL)->monitored))
			return (1);
		if (ft_strcmp(arg[0], "bg"))
		{
			
		}
		else
		{
			//bg shit here
		}
		arg++;
	}
	return (0);
}

int ft_jobs(char **arg)
{
	t_job *j;

	j = sh_config_getter(NULL)->monitored;
	printf("monitored head: %p\n", j);
	while (j)
	{
		printf("%d -- %s\n", j->gid, j->processes->arg[0]);
		j = j->next;
	}
	return (0);
}
