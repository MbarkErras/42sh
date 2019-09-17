/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 18:56:01 by merras            #+#    #+#             */
/*   Updated: 2019/08/03 23:21:59 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	redirections_cleanup(t_redir *redir)
{
	t_redir *temp;

	temp = redir;
	while (temp)
	{
		if (temp->type == O_CREAT || temp->type == O_APPEND || temp->type == O_RDONLY)
			close(temp->dst_fd);
		temp = temp->next;
	}
}

int	apply_redirections(t_redir *redir)
{
	t_redir *temp;

	temp = redir;
	while (temp)
	{
		redir = temp->next;
		if (temp->type == -1)
			dup2(temp->dst_fd, temp->src_fd);
		else if (temp->type == O_CREAT || temp->type == O_APPEND)
		{
			if ((temp->dst_fd = open(temp->file, temp->type | O_WRONLY, 0640)) == -1)
			{
				ft_putendl("Couldn't open file.");
				return (0);
			}
			if (temp->src_fd == BOTH_FDS)
			{
				dup2(temp->dst_fd, 1);
				dup2(temp->dst_fd, 2);
			}
			else
				dup2(temp->dst_fd, temp->src_fd);
			close(temp->dst_fd);
		}
		else if (temp->type == CLOSE_FD)
			close(temp->src_fd);
		else if (temp->type == O_RDONLY)
		{
			if ((temp->dst_fd = open(temp->file, temp->type)) == -1)
			{
				ft_putendl("Couldn't open file.");
				return (0);
			}
			dup2(temp->dst_fd, temp->src_fd);
			close(temp->dst_fd);
		}
		temp = redir;
		free(redir);
	}
	return (1);
}
