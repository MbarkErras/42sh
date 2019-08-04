/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_cd.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/30 20:39:07 by merras            #+#    #+#             */
/*   Updated: 2019/08/02 09:07:55 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

int			cd_home(char ***in)
{
	if (read_env("HOME"))
	{
		free_char_array(*in);
		*in = malloc(sizeof(char *) * 3);
		(*in)[0] = ft_strdup("");
		(*in)[1] = ft_strdup(read_env("HOME"));
		(*in)[2] = NULL;
		return (b_cd(in));
	}
	else
		return (ft_perror(EXEC_NAME, "HOME", N_ENV));
}

int			cd_replace(char ***in, char *old)
{
	char *temp;

	temp = string_replace(old, (*in)[1], (*in)[2]);
	(*in)[1] = temp;
	ft_strdel(&(*in)[2]);
	return (b_cd(in));
}

int			cd_old(char ***in)
{
	if (read_env("OLDPWD"))
	{
		ft_strdel(&(*in)[1]);
		(*in)[1] = ft_strdup(read_env("OLDPWD"));
		return (b_cd(in));
	}
	else
		return (ft_perror(EXEC_NAME, "OLDPWD", N_ENV));
}

static void	cd_chdir(char **in, char *old)
{
	update_env("OLDPWD", old, 1);
	chdir(in[1]);
	update_env("PWD", getcwd(old, 4096), 1);
}

int			b_cd(char ***in)
{
	char		old[4096];
	struct stat	sbuf;

	getcwd(old, 4096);
	stat((*in)[1], &sbuf);
	if (array_length(*in) == 1)
		return (cd_dispatcher(in, NULL, 0));
	else if (array_length(*in) > 3)
		return (-1 + ft_perror((*in)[0], (*in)[1], X_ARG));
	else if (array_length(*in) == 3 && !ft_strstr(old, (*in)[1]))
		return (-1 + ft_perror((*in)[0], (*in)[1], N_PWD));
	else if (array_length(*in) == 3 && ft_strstr(old, (*in)[1]))
		return (cd_dispatcher(in, old, 1));
	else if (array_length(*in) == 2 && !ft_strcmp((*in)[1], "-"))
		return (cd_dispatcher(in, NULL, 2));
	else if (!S_ISDIR(sbuf.st_mode))
		return (-1 + ft_perror((*in)[0], (*in)[1], N_DIR));
	else if (access((*in)[1], F_OK) == -1)
		return (-1 + ft_perror((*in)[0], (*in)[1], N_XST));
	else if (access((*in)[1], X_OK) == -1)
		return (-1 + ft_perror((*in)[0], (*in)[1], N_PRM));
	else
		cd_chdir(*in, old);
	return (0);
}
