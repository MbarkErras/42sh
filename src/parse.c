/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@1337.ma>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 16:25:14 by merras            #+#    #+#             */
/*   Updated: 2019/09/12 17:06:57 by yoyassin         ###   ########.fr       */
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
/* echo "a\""'b"'*/
void		print_parsing_res(t_job *head)
{
	int a = 1;

	while (head)
	{
		printf("job: %d\n", a);
		while (head->processes)
		{
			while (*head->processes->arg)
			{
				printf("\nprocess: %s\n", *head->processes->arg);
				while (head->processes->redir)
				{
					printf("Redir:\n type: %d, src_fd: %d, dst_fd: %d, file: %s\n",
					head->processes->redir->type, head->processes->redir->src_fd, head->processes->redir->dst_fd, head->processes->redir->file);
					head->processes->redir = head->processes->redir->next;
				}
				head->processes->arg++;
			}
			head->processes = head->processes->next;
		}
		printf("FLAG: %d\n", head->flag);
		a++;
		head = head->next;
	}
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
		if (!q && line[i] == '"' && line[i - 1] != UQ_ESCAPE && line[i - 1] != Q_ESCAPE)
			dq = !dq;
		else if (!dq && line[i] == '\'') /* should check escape in single quotes afterwards */
			q = !q;
		if (!q && !dq)
		{
			if (line[i] == ';')
				line[i] = SEMI_COL;
			else if (ft_isspace(line[i]))
				line[i] = BLANK;
			else if (line[i - 1] != UQ_ESCAPE && line[i] == 92)
				line[i] = UQ_ESCAPE;
			else if (line[i] == '|' && line[i + 1] != '|')
				line[i] = PIPE;
			else if (line[i] == '&' && line[i + 1] == '&')
			{
				line[i] = AND;
				line[i + 1] = AND;
			}
			else if (line[i] == '|' && line[i + 1] == '|')
			{
				line[i] = OR;
				line[i + 1] = OR;
			}
			else if (line[i + 1] != '>' && line[i + 1] != '<')
			{
				if (line[i] == '>')
					line[i] = OUT_RED_OP;
				else if (line[i] == '<')
					line[i] = IN_RED_OP;
			}
			else if (line[i] == '>' && line[i + 1] == '>')
			{
				line[i] = APP_OUT_RED_OP;
				line[i + 1] = APP_OUT_RED_OP;
			}
			else if (line[i] == '<' && line[i + 1] == '<')
			{
				line[i] = HEREDOC_OP;
				line[i + 1] = HEREDOC_OP;
			}
		}
		else if (dq && line[i - 1] != Q_ESCAPE && line[i] == 92)
			line[i] = Q_ESCAPE;
	}
}

void			apply_globbing(char **line)
{
	int		i;
	int		pos;
	int		start;
	char	*buf;
	char	*gl_pattern;
	char	*s1;
	char	*s2;
	char	q;
	char	dq;

	pos = 0;
	start = 0;
	gl_pattern = NULL;
	s1 = NULL;
	s2 = NULL;
	q = 0;
	dq = 0;
	i = -1;
	while ((*line)[++i])
	{
		if (!q && (*line)[i] == '"') /*should add escape */
			dq = !dq;
		else if (!dq && (*line)[i] == '\'')
			q = !q;
		if (!q && !dq && (*line)[i] == '*')
		{
			pos = i;
			while (i > 0 && (*line)[i] != BLANK && (*line)[i] != SEMI_COL && (*line)[i] != PIPE && (*line)[i] != OR && (*line)[i] != AND)
				i--;
			start = i + 1;
			i = pos;
			while (i > 0 && (*line)[i] && (*line)[i] != BLANK && (*line)[i] != SEMI_COL && (*line)[i] != PIPE && (*line)[i] != OR && (*line)[i] != AND)
				i++;
			gl_pattern = ft_strsub((*line), start, i - start);
			buf = NULL;
			if (!apply_glob_expansion(gl_pattern, &buf))
			{
				free(gl_pattern);
				continue ;
			}
			for (int i; i < ft_strlen(buf); i++)
				if (buf[i] == ' ')
					buf[i] = BLANK;
			s1 = ft_strsub((*line), 0, start);
			s2 = ft_strsub((*line), i, ft_strlen((*line)) - ft_strlen(s1) + ft_strlen(gl_pattern) - 1);
			(*line) = ft_fstrjoin(s1, buf);
			(*line) = ft_fstrjoin((*line), s2);
			i = pos + ft_strlen(buf) - 1;
			free(gl_pattern);
			free(buf);
		}
	}
}

t_process			*get_cmds_list(char *cmd_chain, t_shell_config *sh)
{
	int			j;
	int			old_j;
	char		*str;
	t_process		*h;
	t_process		*c;
	t_process		*t;

	j = 0;
	h = NULL;
	c = NULL;
	t = NULL;
	old_j = 0;
	str = NULL;
	while (cmd_chain[j])
	{
		if (c && cmd_chain[j] == PIPE)
		{
			c->flag = cmd_chain[j];
			c->next = NULL;
			old_j += j - old_j;
			j++;
			if (!h)
			{
				h = c;
				t = c;
			}
			else
			{
				t->next = c;
				t = c;
			}
			free(str);
		}
		else
		{
			old_j += j - old_j;
			while (cmd_chain[j] && cmd_chain[j] != PIPE)
				j++;
			str = ft_strsub(cmd_chain, old_j, j - old_j);
			// printf("str: %s\n",str);
			c = (t_process *)malloc(sizeof(t_process));
			c->heredoc = NULL;
			check_redirections(str, c, sh);
			// free(str);
			apply_expansions(c->arg);
			c->flag = 0;
			c->next = NULL;
		}
	}
	if (!h)
	{
		h = c;
		t = c;
	}
	else
	{
		t->next = c;
		t = c;
	}
	// free(str);
	return (h);
}

/*
**	-Check if words are properly quoted.
**	-Mark the operators '|' '||' '&&' '>' ... + spaces.
**	-Apply globbing '*' only.
**	-Error checking.
**	-Should fix redir syntax errors.
*/

int				is_not_blank(char *line, int j, int i)
{
	char	valid;

	valid = 0;
	while (j < i)
	{
		if (line[j] != BLANK)
			valid = 1;
		j++;
	}
	return (valid);
}

int				get_operands(char *line, int *i, char t_op, int *o_i)
{
	int		j;
	char	operands;

	operands = 0;
	*o_i = *i;
	if (!(*i) && (IS_REDIR_OP(*i, _AND, _NEQ)))
		return (operands);
	j = (*i - 1) >= 0 ? (*i - 1) : *i;
	if ((j >= 0 && IS_OPERATOR(j, _AND, _NEQ)) || (!j && (IS_REDIR_OP(j, _OR, _EQ))))
	{
		while (j && IS_OPERATOR(j, _AND, _NEQ))
			j--;
		if (IS_OPERATOR(j, _OR, _EQ))
			j++;
		if ((j != *i - 1) && is_not_blank(line, j, *i))
			operands |= LEFT_OPR;
		t_op ? (j = *i + 2)
		: (j = *i + 1);
		while ((line[j] && IS_OPERATOR(j, _AND, _NEQ)) || IS_REDIR_OP(j, _OR, _EQ))
			j++;
		if ((j != *i + 1) && is_not_blank(line, *i = t_op ? (*i + 2) : (*i + 1), j))
		{
			*i = j - 2;
			operands |= RIGHT_OPR;
		}
	}
	return (operands);
}
/*
** Needs more testing.
*/
int				check_syntax_errors(char *line)
{
	int		i;
	char	t_op;
	char	ops;
	int		j;

	i = 0;
	while (line[i])
	{
		t_op = 0;
		if (IS_OPERATOR(i, _OR, _EQ))
		{
			if (TWO_C_OPS(i, _OR, _EQ))
				t_op = 1;
			if ((ops = get_operands(line, &i, t_op, &j)) == 1 && (line[j] == SEMI_COL))
			{
				i++;
				continue ;
			}
			else if ((IS_REDIR_OP(j, _OR, _EQ)) && (ops & RIGHT_OPR) == RIGHT_OPR)
			{
				i++;
				continue ;
			}
			else if ((ops & RIGHT_OPR) != RIGHT_OPR || (ops & LEFT_OPR) != LEFT_OPR)
			{
				ft_putendl("syntax error.");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

t_job		*parse(t_shell_config *sh)
{
	char		**cmd_chain;
	char		*line;
	t_job	*curr;
	t_job	*head;
	t_job	*tail;

	head = NULL;
	tail = NULL;
	line = pre_parse(sh);
	sh->in = ft_strdup(line);
	mark_operators(line);
	if (check_syntax_errors(line))
		return (NULL);
	apply_globbing(&line);
	// printf("line: %s\n", line);
	cmd_chain = ft_strsplit(line, SEMI_COL);
	int	j;
	int	old_j = 0;
	char *token = NULL;
	curr = NULL;
	while (*cmd_chain)
	{
		if (is_not_blank(*cmd_chain, 0, ft_strlen(*cmd_chain)))
		{
			j = 0;
			while ((*cmd_chain)[j])
			{
				if (curr && ((*cmd_chain)[j] == AND || (*cmd_chain)[j] == OR))
				{
					curr->flag = (*cmd_chain)[j];
					old_j += j - old_j;
					j += 2;
					curr->next = NULL;
					if (!head)
					{
						head = curr;
						tail = curr;
					}
					else
					{
						tail->next = curr;
						tail = curr;
					}
					free(token);
				}
				else
				{
					old_j += j - old_j;
					while ((*cmd_chain)[j] && (*cmd_chain)[j] != AND && (*cmd_chain)[j] != OR)
						j++;
					token = ft_strsub(*cmd_chain, old_j, j - old_j);
					// printf("token: %s\n", token);
					if (!(curr = (t_job *)malloc(sizeof(t_job))))
						exit(2);
					curr->processes = get_cmds_list(token, sh);
					curr->flag = 0;
					curr->next = NULL;
				}
			}
			if (!head)
			{
				head = curr;
				tail = curr;
			}
			else
			{
				tail->next = curr;
				tail = curr;
			}
		}
		cmd_chain++;
	}
	// while (head->processes->redir)
	// {
	// 	printf("Redir:\n type: %d, src_fd: %d, dst_fd: %d, file: %s\n",
	// 	head->processes->redir->type, head->processes->redir->src_fd, head->processes->redir->dst_fd, head->processes->redir->file);
	// 	head->processes->redir = head->processes->redir->next;
	// }
	// t_job *tmp = head;
	// print_parsing_res(tmp);
	return (head);
}
