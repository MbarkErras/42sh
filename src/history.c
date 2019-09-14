/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/03 21:31:42 by merras            #+#    #+#             */
/*   Updated: 2019/08/18 12:51:46 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

int	history_dispatcher(char c, char **in)
{
	if (c == 'c')
		list_delete(sh_config_getter(NULL)->hist, hist_node_del);
	else if (c == 'd')
		return (history_delete(in));
	else if (c == 'a' || c == 'w')
		return (history_write(in, c));
	else if (c == 'r' || c == 'n')
		return ();
	else if (c == 'p')
		return ();
	else if (c == 's')
		return ();
	return (0);
}

int	history_turing_iterator(char *flagset, char **in)
{
	int ret;

	while (*flagset)
	{
		if ((ret = history_dispatcher(*flagset, in)))
			return (ft_perror(EXEC_NAME, NULL, 0));
		flagset++;
	}
	return (0);
}

int	b_history(char **in)
{
	int i;
	int j;
	char flagset[9];

	if (!in[1])
		return (print_history());
	i = 0;
	ft_memset(flags, 0, sizeof(flags));
	while (in[++i][0] == '-')
	{
		if (!is_flag(in[i] + 1, "cdanrwps"))
			return (ft_perror(EXEC_NAME, in[i], N_PRM));
		setter(flagset, in[i] + 1);
	}
	return (history_turing_iterator(flagset, in + i));
}
