/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 16:25:14 by merras            #+#    #+#             */
/*   Updated: 2019/09/20 12:05:45 by yoyassin         ###   ########.fr       */
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
	int a = 1;
	int	b = 1;
	int	j = 1;

	while (head)
	{
		printf("job: %d\n", j);
		a = 1;
		while (head->proc_gr)
		{
			printf("pr_grp: %d\n", a);
			b = 1;
			while (head->proc_gr->processes)
			{
				printf("process nb: %d\n", b);
				while (*head->proc_gr->processes->arg)
				{
					printf("\np_arg: %s\n", *head->proc_gr->processes->arg);
					while (head->proc_gr->processes->redir)
					{
						printf("Redir:\n type: %d, src_fd: %d, dst_fd: %d, file: %s\n",
						head->proc_gr->processes->redir->type, head->proc_gr->processes->redir->src_fd, head->proc_gr->processes->redir->dst_fd, head->proc_gr->processes->redir->file);
						head->proc_gr->processes->redir = head->proc_gr->processes->redir->next;
					}
					head->proc_gr->processes->arg++;
				}
				head->proc_gr->processes = head->proc_gr->processes->next;
				b++;
			}
			printf("FLAG: %d\n", head->proc_gr->flag);
			a++;
			head->proc_gr = head->proc_gr->next;
		}
		printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		j++;
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
	else if (line[i - 1] != UQ_ESCAPE && line[i] == 92)
		return ((line[i] = UQ_ESCAPE));
	else if (line[i] == '|' && line[i + 1] != '|')
		return ((line[i] = PIPE));
	else if ((line[i] == '>' || line[i] == '<')
	&& (line[i + 1] != '>' && line[i + 1] != '<'))
		return ((line[i] = (line[i] == '>' ? OUT_RED_OP : IN_RED_OP)));
	else if (line[i] == '&' && line[i + 1] != '&')
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
		if (!q && line[i] == '"' && line[i - 1] != UQ_ESCAPE)
			dq = !dq;
		else if (!dq && line[i] == '\'' && line[i - 1] != UQ_ESCAPE)
			q = !q;
		if (!q && !dq && line[i - 1] != UQ_ESCAPE)
		{
			if (sc_operator(line, i) || dc_operator(line, i))
				continue ;
			else if (ft_isspace(line[i]))
				line[i] = BLANK;
		}
		else if (dq && line[i - 1] != Q_ESCAPE && line[i] == 92)
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
		if (!q && line[i] == '"' && line[i - 1] != UQ_ESCAPE)
			dquotes_checker(&line, &dq, &i, &j);
		else if (!dq && line[i] == '\'' && line[i - 1] != UQ_ESCAPE)
			squotes_checker(&line, &q, &i);
		else if (!q && !dq && line[i - 1] != UQ_ESCAPE && line[i] == 92)
			line[i] = UQ_ESCAPE;
		i++;
	}
	return (line);
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
	apply_globbing(&line);
	cmd_chain = ft_strsplit(line, SEMI_COL);
	head = get_jobs(cmd_chain);
	t_job *tmp = head;
	print_parsing_res(tmp);
	return (head);
}

	// while (head->processes->redir)
	// {
	// 	printf("Redir:\n type: %d, src_fd: %d, dst_fd: %d, file: %s\n",
	// 	head->processes->redir->type, head->processes->redir->src_fd, head->processes->redir->dst_fd, head->processes->redir->file);
	// 	head->processes->redir = head->processes->redir->next;
	// }