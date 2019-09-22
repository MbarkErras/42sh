/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_drivers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/09 17:27:45 by merras            #+#    #+#             */
/*   Updated: 2019/09/22 22:12:03 by mmostafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

char	*string_replace(char *src, char *find, char *replace)
{
	char	*new;
	size_t	start;
	size_t	i;
	int		j;

	new = ft_strnew(ft_strlen(src) + ft_strlen(replace) - ft_strlen(find));
	start = (int)(ft_strstr(src, find) - src);
	i = -1;
	j = -1;
	while (++i < start)
		new[i] = src[i];
	while (replace[++j])
		new[i++] = replace[j];
	start += ft_strlen(find) - 1;
	while (++start < ft_strlen(src))
		new[i++] = src[start];
	return (new);
}

int		cd_dispatcher(char ***in, char *arg, int f)
{
	if (f == 0)
		return (cd_home(in));
	else if (f == 1)
		return (cd_replace(in, arg));
	else if (f == 2)
		return (cd_old(in));
	return (0);
}

int		b_env_updater(char **in)
{
	char	*equal_mark;
	int		i;

	i = 0;
	while (in[++i] &&
			(equal_mark = ft_strchr(in[i], '=')) && equal_mark != in[i])
	{
		*equal_mark = '\0';
		update_env(in[i], equal_mark + 1, 1);
	}
	return (i);
}

int		is_builtin(char *cmd)
{
	if (!ft_strcmp("echo", cmd) ||
		!ft_strcmp("cd", cmd) ||
		!ft_strcmp("setenv", cmd) ||
		!ft_strcmp("unsetenv", cmd) ||
		!ft_strcmp("exit", cmd) ||
		!ft_strcmp("env", cmd) ||
		!ft_strcmp("export", cmd) ||
		!ft_strcmp("set", cmd) /*||
		!ft_strcmp("unset", cmd)*/)
		return (1);
	return (0);
}

int		builtins_dispatcher(char ***arg)
{
	if (!ft_strcmp("echo", (*arg)[0]))
		return (b_echo(*arg));
	if (!ft_strcmp("cd", (*arg)[0]))
		return (b_cd(arg));
	if (!ft_strcmp("setenv", (*arg)[0]))
		return (b_senv(*arg, 1));
	if (!ft_strcmp("unsetenv", (*arg)[0]))
		return (b_senv(*arg, 0));
	if (!ft_strcmp("env", (*arg)[0]))
		return (b_env(*arg));
	if (!ft_strcmp("exit", (*arg)[0]))
		b_exit(*arg);
	if (!ft_strcmp("export", (*arg)[0]))
		return (ft_export(*arg));
	if (!ft_strcmp("set", (*arg)[0]))
		return (ft_set(*arg));
	/*if (!ft_strcmp("unset", (*arg)[0]))
		return (ft_unset(*arg));*/
	return (0);
}
