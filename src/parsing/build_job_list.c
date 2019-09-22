/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_job_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 18:46:41 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/21 23:14:26 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

t_job				*build_jobs_lst(void **head, void **tail, char *cmd, int bg)
{
	t_job	*curr;
	char	*token;
	int		old_j;
	int		j;

	curr = NULL;
	token = NULL;
	j = 0;
	while (cmd[j])
	{
		if (curr && (cmd[j] == BG || (!bg && (cmd[j] == AND || cmd[j] == OR))))
		{
			old_j += j - old_j;
			j += set_flag((void *)curr, cmd[j], 2);
			bg -= bg ? 1 : 0;
			APPEND(1);
			curr = NULL;
			free(token);
		}
		else
			get_list_node(bg ? 2 : 1, (void *)&curr,
			(token = skip_operators(bg ? 2 : 1, cmd, &old_j, &j)));
	}
	return (curr);
}

t_job				*get_jobs(char **cmd_chain, int bg)
{
	t_job	*head;
	t_job	*curr;
	t_job	*tail;

	curr = NULL;
	head = NULL;
	tail = NULL;
	while (*cmd_chain)
	{
		if (is_not_blank(*cmd_chain, 0, ft_strlen(*cmd_chain)))
		{
			curr = build_jobs_lst((void *)&head, (void *)&tail, *cmd_chain, bg);
			append((void *)&head, (void *)&curr, (void *)&tail, 1);
		}
		cmd_chain++;
	}
	return (head);
}
