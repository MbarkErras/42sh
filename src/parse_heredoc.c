/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/11 21:33:24 by yoyassin          #+#    #+#             */
/*   Updated: 2019/08/11 21:33:47 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/mshell.h"

char	*get_heredoc_string(char *eof, t_shell_config *sh)
{
	char	*buf;
	char	*tmp;

	buf = ft_strnew(0);
	while (1)
	{
		read_cline("heredoc> ", sh);
		if (ft_strequ(sh->in, eof))
			break ;
		tmp = buf;
		buf = ft_strjoin(buf, sh->in);
		free(tmp);
		buf = ft_fstrjoin(buf, ft_strdup("\n"));
	}
	return (buf);
}

int		get_heredoc_fd(char *str, int i)
{
	while (str[i] && ft_isdigit(str[--(i)]))
		;
	return (ft_atoi(str + i + 1));
}

char	*get_heredoc(char *str, int *i, t_shell_config *sh, int *hd_fd)
{
	char	*eof;
	int		old_i;

	old_i = *i;
	*hd_fd = (str[*i - 1] == BLANK || ft_isalpha(str[*i - 1])) ?
	0 : get_heredoc_fd(str, *i);
	*i = old_i;
	ft_memset(str + *i, BLANK, 2);
	*i += 2;
	old_i = *i;
	while (str[*i] == BLANK)
	{
		old_i++;
		(*i)++;
	}
	while (ft_isalnum(str[*i]))
		(*i)++;
	eof = ft_strsub(str, old_i, *i - old_i);
	ft_memset(str + old_i, BLANK, *i - old_i);
	return (get_heredoc_string(eof, sh));
}
