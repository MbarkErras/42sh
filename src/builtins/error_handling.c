/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 14:40:37 by merras            #+#    #+#             */
/*   Updated: 2019/08/07 10:25:53 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

char	*ft_strerror(int err)
{
	if (err == N_PRM)
		return (N_PRM_T);
	if (err == N_DIR || err == N_XST)
		return (err == N_DIR ? N_DIR_T : N_XST_T);
	if (err == X_ARG || err == N_ARG)
		return (err == X_ARG ? X_ARG_T : N_ARG_T);
	if (err == N_CHN || err == N_PWD)
		return (err == N_CHN ? N_CHN_T : N_PWD_T);
	if (err == N_TRM || err == N_EXE)
		return (err == N_TRM ? N_TRM_T : N_EXE_T);
	if (err == A_TRM || err == S_TRM)
		return (err == A_TRM ? A_TRM_T : S_TRM_T);
	if (err == N_TTY || err == F_EXE)
		return (err == N_TTY ? N_TTY_T : F_EXE_T);
	if (err == I_DIR || err == N_ENV)
		return (err == I_DIR ? I_DIR_T : N_ENV_T);
	if (err == F_PIP || err == F_FRK)
		return (err == F_PIP ? F_PIP_T : F_FRK_T);
	return (NULL);
}

int		ft_perror(char *command, char *arg, int err)
{
	if (command)
		ft_putstr(command);
	ft_putstr(ft_strerror(err));
	if (arg)
		ft_putendl(arg);
	else
		ft_putchar('\n');
	return (err);
}
