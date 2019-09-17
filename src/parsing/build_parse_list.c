/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_parse_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/17 13:19:43 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/17 13:36:25 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void		append(void **h, void **c, void **t, char type)
{
	if (!*h)
	{
		*h = *c;
		*t = *c;
	}
	else
	{
		if (type)
			((t_job *)(*t))->next = *c;
		else
			((t_process *)(*t))->next = *c;
		*t = *c;
	}
}

int		set_flag(void *curr, int flag, char type)
{
	if (!type)
	{
		((t_process *)curr)->flag = flag;
		((t_process *)curr)->next = NULL;
		return (1);
	}
	else
	{
		((t_job *)curr)->flag = flag;
		((t_job *)curr)->next = NULL;
		return (2);
	}
}

char		*skip_operators(char type, char *token, int *start, int *j)
{
	(*start) += (*j) - (*start);
	if (type)
	{
		while (token[*j] && token[*j] != AND && token[*j] != OR)
			(*j)++;
	}
	else
	{
		while (token[*j] && token[*j] != PIPE)
			(*j)++;
	}
	return (ft_strsub(token, *start, (*j) - (*start)));
}

void	get_list_node(char type, void **curr, char *str)
{
	if (type)
	{
		*curr = (t_job *)malloc(sizeof(t_job));
		((t_job *)*curr)->processes = get_process_list(str);
		((t_job *)*curr)->flag = 0;
		((t_job *)*curr)->next = NULL;
	}
	else
	{
		*curr = (t_process *)malloc(sizeof(t_process));
		((t_process *)*curr)->heredoc = NULL;
		check_redirections(str, ((t_process *)*curr));
		apply_expansions(((t_process *)*curr)->arg);
		((t_process *)*curr)->flag = 0;
		((t_process *)*curr)->next = NULL;
	}
}

void	*build_list(void **head, void **tail, char *token, char type)
{
	int		j;
	int		old_j;
	char	*str;
	void	*curr;

	j = 0;
	old_j = 0;
	str = NULL;
	curr = NULL;
	while (token[j])
	{
		if (curr && ((!type && token[j] == PIPE)
		|| (type && (token[j] == AND || token[j] == OR))))
		{
			old_j += j - old_j;
			j += (!type && token[j] == PIPE) ? SETFLAG(0) : SETFLAG(1);
			(!type) ? APPEND(0) : APPEND(1);
			curr = NULL;
			free(str);
		}
		else
			get_list_node(type, &curr,
			(str = skip_operators(type, token, &old_j, &j)));
	}
	return (curr);
}

t_process			*get_process_list(char *cmd_chain)
{
	t_process		*h;
	t_process		*t;
	void			*c;

	h = NULL;
	t = NULL;
	c = build_list((void *)&h, (void *)&t, cmd_chain, 0);
	append((void *)&h, &c, (void *)&t, 0);
	return (h);
}

t_job		*get_parse_list(char **cmd_chain)
{
	t_job	*head;
	t_job	*tail;
	void	*curr;

	head = NULL;
	tail = NULL;
	while (*cmd_chain)
	{
		if (is_not_blank(*cmd_chain, 0, ft_strlen(*cmd_chain)))
		{
			curr = build_list((void *)&head, (void *)&tail, *cmd_chain, 1);
			append((void *)&head, &curr, (void *)&tail, 1);
		}
		cmd_chain++;
	}
	return (head);
}
