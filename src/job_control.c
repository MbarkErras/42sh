/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 21:25:46 by merras            #+#    #+#             */
/*   Updated: 2019/09/20 05:04:30 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

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

t_job *jobslist_top(t_job *l, int f)
{
	if (!l || (!l->next && f))
		return (NULL);
	if (!l->next && !f)
		return (l);
	if (!l->next->next && f)
		return (l);
	while (f ? l->next->next : l->next)
		l = l->next;
	return (l);
}

int	parse_jobspec(char *jspec)
{
	int spec;
	t_job *monitored;

	if (!jspec)
		return (jobslist_top(sh_config_getter(NULL)->monitored, 0));
	if (jspec[0] != '%' || ft_strlen(jspec) < 2)
		return (0);
	if (jspec[1] == '%' || jspec[1] == '+')
		return (jobslist_top(sh_config_getter(NULL)->monitored, 0));
	if (jspec[2] == '-')
		return (jobslist_top(sh_config_getter(NULL)->monitored, 1));
	if (ft_digitscount((spec = ft_atoi(jspec + 1))) == ft_strlen(jspec + 1))
		return (ft_atoi(jspec + 1));
	monitored = sh_config_getter(NULL)->monitored;
	jspec = 1;
	while (monitored)
	{
		if (!ft_strncmp(monitored->job_string, jspec + 1, ft_strlen(jspec + 1)))
			return (jspec);
		jspec++;
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
}

int ft_jobs(char **arg)
{

}
