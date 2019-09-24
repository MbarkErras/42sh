/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility_funcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 18:50:04 by yoyassin          #+#    #+#             */
/*   Updated: 2019/09/24 12:26:48 by yoyassin         ###   ########.fr       */
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
		if (type == 1)
			((t_job *)(*t))->next = *c;
		else
			((t_process *)(*t))->next = *c;
		*t = *c;
	}
}

int		set_flag(void *curr, int flag, char type)
{
	if (!type || type == 1)
	{
		((t_process *)curr)->flag = flag;
		((t_process *)curr)->jcflags = 0;
		((t_process *)curr)->next = NULL;
		return (flag == PIPE ? 1 : 2);
	}
	else
	{
		((t_job *)curr)->flag = flag;
		((t_job *)curr)->jcflags = flag == BG
		? F_SET(((t_job *)curr)->jcflags, F_BACKGROUND) : 0;
		((t_job *)curr)->next = NULL;
		return (flag == BG ? 1 : 2);
	}
}

char		*skip_operators(char type, char *token, int *start, int *j)
{
	(*start) += (*j) - (*start);
	if (type == 1)
	{
		while (token[*j] && token[*j] != AND && token[*j] != OR)
			(*j)++;
	}
	else if (type == 2)
	{
		while (token[*j] && token[*j] != BG)
			(*j)++;
	}
	else if (type == 3)
	{
		while (token[*j]
		&& token[*j] != PIPE && token[*j] != AND && token[*j] != OR)
			(*j)++;
	}
	else
	{
		while (token[*j] && token[*j] != PIPE)
			(*j)++;
	}
	return (ft_strsub(token, *start, (*j) - (*start)));
}

char	*get_cmd_string(char *s)
{
	char	*str;
	int		i;

	str = ft_strdup(s);
	i = 0;
	while (s[i])
	{
		if (s[i] == BLANK)
			str[i] = ' ';
		else if (s[i] == PIPE)
			str[i] = '|';
		else if (s[i] == OUT_RED_OP)
			str[i] = '>';
		else if (s[i] == IN_RED_OP)
			str[i] = '<';
		else if (s[i] == APP_OUT_RED_OP)
		{
			str[i] = '>';
			str[i + 1] = '>';
		}
		else if (s[i] == HEREDOC_OP)
		{
			str[i] = '<';
			str[i + 1] = '<';
		}
		else if (s[i] == OR)
		{
			str[i] = '|';
			str[i + 1] = '|';
		}
		else if (s[i] == AND)
		{
			str[i] = '&';
			str[i + 1] = '&';
		}
		i++;
	}
	return (str);
}

void	get_list_node(char type, void **curr, char *str)
{
	if (type)
	{
		*curr = (t_job *)malloc(sizeof(t_job));
		((t_job *)*curr)->processes = get_process_list(str, type);
		((t_job *)*curr)->command = get_cmd_string(str);
		((t_job *)*curr)->gid = 0;
		((t_job *)*curr)->flag = 0;
		((t_job *)*curr)->next = NULL;
	}
	else
	{
		*curr = (t_process *)malloc(sizeof(t_process));
		((t_process *)*curr)->heredoc = NULL;
		check_redirections(str, ((t_process *)*curr));
		apply_expansions(((t_process *)*curr)->arg);
		((t_process *)*curr)->jcflags = 0;
		((t_process *)*curr)->flag = 0;
		((t_process *)*curr)->next = NULL;
	}
}
