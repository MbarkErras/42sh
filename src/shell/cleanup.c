/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 09:46:51 by merras            #+#    #+#             */
/*   Updated: 2019/08/04 00:01:30 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	exit_cleanup(int exit_status, int err)
{
	t_shell_config *sh;

	sh = sh_config_getter(NULL);
	if (err != -1)
		ft_perror(EXEC_NAME, NULL, err);
	ft_putchar('\n');
	free(env_converter());
	exit(exit_status);
}
