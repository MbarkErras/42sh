/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 04:19:50 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/21 18:58:11 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

static void		append_redir(t_redir **h, t_redir **c, t_redir **t)
{
	if (!*h)
	{
		*h = *c;
		*t = *c;
	}
	else
	{
		(*t)->next = *c;
		*t = *c;
	}
}

/*
** should do error checking when parsing is done
*/

static t_redir	*get_node(void)
{
	t_redir *node;

	if (!(node = (t_redir *)malloc(sizeof(t_redir))))
		exit(2);
	node->next = NULL;
	return (node);
}

void			get_redir_type(t_redir *curr, char *str, int i)
{
	if (str[i] == OUT_RED_OP || str[i] == IN_RED_OP)
		curr->type = (str[i] == OUT_RED_OP) ? O_CREAT : O_RDONLY;
	else
		curr->type = O_APPEND;
	if (str[i + 1] != '&')
	{
		if (curr->type != O_APPEND && str[i - 1] == '&')
		{
			curr->src_fd = BOTH_FDS;
			str[i - 1] = BLANK;
		}
	}
	else if (curr->type != O_APPEND && str[i + 2] && str[i + 1] == '&')
		curr->type = FD_AGGR;
}

int			get_redir_info(t_redir *curr, char *str, int *i)
{
	curr->dst_fd = 0;
	get_redir_type(curr, str, *i);
	if (curr->src_fd != BOTH_FDS)
	{
		if (!get_redir_fds(curr, str, i))
			return (0);
	}
	get_redir_file(curr, str, i);
	return (1);
}

char		*check_redirections(char *str, t_process *cmd)
{
	int		i;
	t_redir	*curr;
	t_redir	*head;
	t_redir	*tail;

	cmd->arg = NULL;
	head = NULL;
	curr = NULL;
	tail = NULL;
	i = -1;
	while (str[++i])
	{
		if (IS_REDIRECTION)
		{
			curr = get_node();
			append_redir(&head, &curr, &tail);
			if (!get_redir_info(curr, str, &i))
				continue ;
		}
		else if (str[i] == HEREDOC_OP)
			cmd->heredoc = get_heredoc(str, &i, &cmd->heredoc_fd);
	}
	cmd->redir = head;
	cmd->arg = ft_strsplit(str, BLANK);
	return (str);
}
