/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 16:25:14 by merras            #+#    #+#             */
/*   Updated: 2019/09/23 22:21:21 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/mshell.h"

int	ft_strpos(char *s1, char *s2)
{
	char *sub;

	if (!(sub = ft_strstr(s1, s2)))
		return (-1);
	return (ft_strlen(s1) - ft_strlen(sub));
}

int		ft_isspace(int c)
{
	unsigned char s;

	s = c;
	if (s == ' ' || s == '\t'
		|| s == '\v' || s == '\r' || s == '\f')
		return (1);
	return (0);
}

void		print_parsing_res(t_job *head)
{
	int	p = 1;
	int	bj = 1;
	int	j = 1;

	while (head)
	{
		if (head->flag == BG)
		{
			printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			printf("background job processes: %d\n", bj);
			p = 1;
			while (head->processes)
			{
				printf("process: %d\n", p);
				while (*head->processes->arg)
				{
					printf("\targ: %s\n", *head->processes->arg);
					head->processes->arg++;
				}
				printf("redirections:\n");
				while (head->processes->redir)
				{
					printf("\ttype: %d\n", head->processes->redir->type);
					head->processes->redir = head->processes->redir->next;
				}
				printf("flag: %d\n", head->processes->flag);
				head->processes = head->processes->next;
				p++;
			}
			bj++;
			printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
		}
		else
		{
			printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
			printf("foreground job processes: %d\n", j);
			p = 1;
			while (head->processes)
			{
				printf("process: %d\n", p);
				while (*head->processes->arg)
				{
					printf("\targ: %s\n", *head->processes->arg);
					head->processes->arg++;
				}
				printf("redirections:\n");
				while (head->processes->redir)
				{
					printf("\ttype: %d\n", head->processes->redir->type);
					head->processes->redir = head->processes->redir->next;
				}
				printf("flag: %d\n", head->processes->flag);
				head->processes = head->processes->next;
				p++;
			}
			printf("flag: %d\n", head->flag);
			j++;
			printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
		}
		head = head->next;
	}
}

/*
**	-Check if words are properly quoted.
**	-Mark the operators '|' '||' '&&' '>' ... + spaces.
**	-Apply globbing '*' only.
**	-Error checking.
**	-Should fix redir syntax errors.
*/

int				dc_operator(char *line, int i)
{
	if (line[i] == '&' && line[i + 1] == '&')
	{
		line[i] = AND;
		return ((line[i + 1] = AND));
	}
	else if (line[i] == '|' && line[i + 1] == '|')
	{
		line[i] = OR;
		return ((line[i + 1] = OR));
	}
	else if (line[i] == '>' && line[i + 1] == '>')
	{
		line[i] = APP_OUT_RED_OP;
		return ((line[i + 1] = APP_OUT_RED_OP));
	}
	else if (line[i] == '<' && line[i + 1] == '<')
	{
		line[i] = HEREDOC_OP;
		return ((line[i + 1] = HEREDOC_OP));
	}
	return (0);
}

int				sc_operator(char *line, int i)
{
	if (line[i] == ';')
		return ((line[i] = SEMI_COL));
	else if (line[i] == 92 && line[(i - 1 > 0) ? i - 1 : 0] != UQ_ESCAPE)
		return ((line[i] = UQ_ESCAPE));
	else if (line[i] == '|' && line[i + 1] != '|')
		return ((line[i] = PIPE));
	else if ((line[i] == '>' || line[i] == '<')
	&& (line[i + 1] != '>' && line[i + 1] != '<'))
		return ((line[i] = (line[i] == '>' ? OUT_RED_OP : IN_RED_OP)));
	else if (line[i + 1] != '>' && line[i + 1] != '<' &&
	IS_REDIR_OP(i + 1, _AND, _NEQ) &&
	IS_REDIR_OP((i - 1 > 0) ? i - 1 : 0, _AND, _NEQ) &&
	line[i] == '&' && line[i + 1] != '&')
		return ((line[i] = BG));
	return (0);
}

void			mark_operators(char *line)
{
	int		i;
	char	q;
	char	dq;

	q = 0;
	dq = 0;
	i = -1;
	while (line[++i])
	{
		if (!q && line[i] == '"' && line[(i - 1 > 0) ? i - 1 : 0] != UQ_ESCAPE)
			dq = !dq;
		else if (!dq && line[i] == '\'' &&
		line[(i - 1 > 0) ? i - 1 : 0] != UQ_ESCAPE)
			q = !q;
		if (!q && !dq && line[(i - 1 > 0) ? i - 1 : 0] != UQ_ESCAPE)
		{
			if (sc_operator(line, i) || dc_operator(line, i))
				continue ;
			else if (ft_isspace(line[i]))
				line[i] = BLANK;
		}
		else if (dq && line[(i - 1 > 0) ? i - 1 : 0] != Q_ESCAPE
		&& line[i] == 92)
			line[i] = Q_ESCAPE;
	}
}

int				is_not_blank(char *line, int j, int i)
{
	char	valid;

	valid = 0;
	while (j < i)
	{
		if (ft_isprint((int)line[j]))
		{
			valid = 1;
			break ;
		}
		j++;
	}
	return (valid);
}

char		*pre_parse(t_shell_config *sh)
{
	char	*line;
	int		i;
	int		j;
	char	q;
	char	dq;

	q = 0;
	dq = 0;
	i = 0;
	j = 0;
	line = ft_strdup(sh->in);
	while (line[i])
	{
		if (!q && line[i] == '"' && line[(i - 1 > 0) ? i - 1 : 0] != UQ_ESCAPE)
			dquotes_checker(&line, &dq, &i, &j);
		else if (!dq && line[i] == '\'' &&
		line[(i - 1 > 0) ? i - 1 : 0] != UQ_ESCAPE)
			squotes_checker(&line, &q, &i);
		else if (!q && !dq && line[i] == 92 &&
		line[(i - 1 > 0) ? i - 1 : 0] != UQ_ESCAPE)
			line[i] = UQ_ESCAPE;
		i++;
	}
	return (line);
}

int			get_bg_jobs(char *line)
{
	int	i;
	int	bg;

	i = 0;
	bg = 0;
	while (line[i])
	{
		if (line[i] == BG)
			bg++;
		i++;
	}
	return (bg);
}

t_job		*parse(t_shell_config *sh)
{
	char		**cmd_chain;
	char		*line;
	t_job		*head;

	head = NULL;
	line = pre_parse(sh);
	sh->in = ft_strdup(line);
	mark_operators(line);
	if (check_syntax_errors(line))
		return (NULL);
	check_wildcard_c(&line);
	cmd_chain = ft_strsplit(line, SEMI_COL);
	head = get_jobs(cmd_chain, get_bg_jobs(line));
	t_job *tmp = head;
	print_parsing_res(tmp);
	return (head);
}
