/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/27 16:25:14 by merras            #+#    #+#             */
/*   Updated: 2019/08/07 04:12:09 by merras           ###   ########.fr       */
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
	
	while (head)
	{
		printf("processes: %d\n", a);
		while (head->processes)
		{
			while (*head->processes->arg)
			{
				printf("\narg: %s\n", *head->processes->arg);
				while (head->processes->redir)
				{
					printf("Redir:\n type: %d, src_fd: %d, dst_fd: %d, file: %s\n",
					head->processes->redir->type, head->processes->redir->src_fd, head->processes->redir->dst_fd, head->processes->redir->file);
					head->processes->redir = head->processes->redir->next;
				}
				head->processes->arg++;
			}
			printf("FLAG: %c\n", head->processes->flag);
			head->processes = head->processes->next;
		}
		a++;
		head = head->next;
	}
}

char	*check_redirections(char *str, t_process *cmd, t_shell_config *sh)
{
	char	q;
	char	dq;
	int		i;
	t_redir	*curr;
	t_redir	*head;
	t_redir	*tail;
	int		length;
	int		old_i;
	char	*buf;
/*
** should do error checking when parsing is done
*/
	q = 0;
	dq = 0;
	i = 0;
	old_i = 0;
	cmd->arg = NULL;
	head = NULL;
	curr = NULL;
	tail = NULL;
	i = 0;
	while (str[i])
	{
		if (!q && str[i] == '"')
			dq = !dq;
		else if (!dq && str[i] == '\'')
			q = !q;
		if (!q && !dq)
		{
			if ((str[i] == '>' && str[i + 1] != '>') || (str[i] == '<' && str[i + 1] != '<'))
			{
				if (!(curr = (t_redir *)malloc(sizeof(t_redir))))
					exit(2);
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
				curr->dst_fd = 0;
				if (str[i] == '>')
					curr->type = O_CREAT;
				else
					curr->type = O_RDONLY;
				if ((ft_isdigit(str[i - 1]) || str[i - 1] == BLANK || ft_isalpha(str[i - 1])) && str[i + 1] != '&')
				{
					if (str[i - 1] == BLANK || ft_isalpha(str[i - 1]))
					{
						if (str[i] == '<')
							curr->src_fd = 0;
						else
							curr->src_fd = 1;
					}
					else
					{
						curr->src_fd = ft_atoi(&str[i - 1]);
						str[i - 1] = BLANK;
					}
				}
				else if (str[i - 1] == '&')
				{
					curr->src_fd = BOTH_FDS;
					str[i - 1] = BLANK;
				}
				else if (str[i + 2] && str[i + 1] == '&')
				{
					curr->type = -1;
					curr->src_fd = ft_atoi(&str[i - 1]);
					if (ft_isdigit(str[i + 2]))
						curr->dst_fd = ft_atoi(&str[i + 2]);
					else if (str[i + 2] == '-')
						curr->type = CLOSE_FD;
					str[i] = BLANK;
					str[i - 1] = BLANK;
					str[i + 1] = BLANK;
					str[i + 2] = BLANK;
					i += 2;
					continue ;
				}
				length = 0;
				str[i] = BLANK;
				old_i = i;
				while (str[i] == BLANK)
				{
					old_i++;
					i++;
				}
				while (ft_isalnum(str[i]))
				{
					length++;
					i++;
				}
				curr->file = ft_strsub(str, old_i, length);
				int j = old_i;
				int k = 0;
				while (k < length)
				{
					str[j] = BLANK;
					j++;
					k++;
				}
				i--;
			}
			else if (str[i] == '>' && str[i + 1] == '>')
			{
				if (!(curr = (t_redir *)malloc(sizeof(t_redir))))
					exit(2);
				curr->type = O_APPEND;
				if ((ft_isdigit(str[i - 1]) || str[i - 1] == BLANK || ft_isalpha(str[i - 1])) && str[i + 2] != '&')
				{
					if (str[i - 1] == BLANK || ft_isalpha(str[i - 1]))
						curr->src_fd = 1;
					else
					{
						curr->src_fd = ft_atoi(&str[i - 1]);
						str[i - 1] = BLANK;
					}
				}
				else if (str[i - 1] == '&')
					ft_putendl_fd("parse error.", 2);
				else if (str[i + 2] && str[i + 2] == '&')
					ft_putendl_fd("parse error.", 2);
				length = 0;
				str[i] = BLANK;
				str[i + 1] = BLANK;
				old_i = i;
				while (str[i] == BLANK)
				{
					old_i++;
					i++;
				}
				while (ft_isalnum(str[i]))
				{
					length++;
					i++;
				}
				curr->file = ft_strsub(str, old_i, length);
				int j = old_i;
				int k = 0;
				while (k < length)
				{
					str[j] = BLANK;
					j++;
					k++;
				}
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
			}
			else if (str[i] == '<' && str[i + 1] == '<')
			{
				str[i] = BLANK;
				str[i + 1] = BLANK;
				i += 2;
				old_i = i;
				while (str[i] == BLANK)
				{
					old_i++;
					i++;
				}
				while (ft_isalnum(str[i]))
					i++;
				char *eof = ft_strsub(str, old_i, i - old_i);
				ft_memset(str + old_i, BLANK, i - old_i);
				buf = ft_strnew(0);
				while (1)
				{
					read_cline("heredoc> ", sh);
					if (ft_strequ(sh->in, eof))
						break ;
					buf = ft_strjoin(buf, sh->in);
					buf = ft_strjoin(buf, "\n");
				}
				cmd->heredoc = ft_strdup(buf);
			}
		}
		i++;
	}
	cmd->redir = head;
	if (!cmd->arg)
		cmd->arg = ft_strsplit(str, BLANK);
	return (str);
}

char		*pre_parse(t_shell_config *sh)
{
	char	*endq;
	char	*line;
	int 	done;
	int		i;
	char	q;
	char	dq;
	char	*tmp;

	q = 0;
	dq = 0;
	i = 0;
	done = 0;
	endq = NULL;
	tmp = NULL;
	line = ft_strdup(sh->in);
	while (line[i])
	{
		if (!q && line[i] == '"')
		{
			dq = !dq;
			if ((endq = ft_strchr(line + i + 1, '"')))
			{
				dq = !dq;
				i = ft_strlen(line) - ft_strlen(endq);
			}
			while (dq)
			{
				line = ft_fstrjoin(line, ft_strdup("\n"));
				read_cline("dq> ", sh);
				tmp = line;
				line = ft_strjoin(line, sh->in);
				free(tmp);
				if (ft_strchr(sh->in, '"'))
				{
					dq = !dq;
					done = 1;
					break ;
				}
			}
		}
		else if (!dq && line[i] == '\'')
		{
			q = !q;
			if ((endq = ft_strchr(line + i + 1, '\'')))
			{
				q = !q;
				i = ft_strlen(line) - ft_strlen(endq);
			}
			while (q)
			{
				line = ft_fstrjoin(line, ft_strdup("\n"));
				read_cline("q> ", sh);
				tmp = line;
				line = ft_strjoin(line, sh->in);
				free(tmp);
				if (ft_strchr(sh->in, '\''))
				{
					q = !q;
					done = 1;
					break ;
				}
			}
		}
		if (done)
			break ;
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
		if (!q && line[i] == '"')
			dq = !dq;
		else if (!dq && line[i] == '\'')
			q = !q;
		if (!q && !dq)
		{
			if (line[i] == ';')
				line[i] = SEMI_COL;
			else if (ft_isspace(line[i]))
				line[i] = BLANK;
			else if (line[i + 1] && line[i] == '|' && line[i + 1] != '|')
				line[i] = PIPE;
			else if (line[i + 1] && line[i] == '&' && line[i + 1] == '&')
			{
				line[i] = AND;
				line[i + 1] = AND;
			}
			else if (line[i + 1] && line[i] == '|' && line[i + 1] == '|')
			{
				line[i] = OR;
				line[i + 1] = OR;
			}
		}
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
		if (!q && (*line)[i] == '"')
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

t_process			*get_processs_list(char *cmd_processes, t_shell_config *sh)
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
	while (cmd_processes[j])
	{
		if (c && (cmd_processes[j] == PIPE || cmd_processes[j] == AND || cmd_processes[j] == OR))
		{
			c->flag = cmd_processes[j];
			c->next = NULL;
			old_j += j - old_j;
			if (c->flag == PIPE)
				j++;
			else
				j += 2;
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
			while (cmd_processes[j] && cmd_processes[j] != AND
			&& cmd_processes[j] != OR && cmd_processes[j] != PIPE)
				j++;
			str = ft_strsub(cmd_processes, old_j, j - old_j);
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
	free(str);
	return (h);
}

t_job		*parse(t_shell_config *sh)
{
	char		**cmd_processes;
	char		*line;
	t_job		*curr;
	t_job		*head;
	t_job		*tail;

	head = NULL;
	tail = NULL;

/*
**	Pre parsing:	-Checking if words are properly quoted.
					-Marking the operators '|' '||' '&&' and spaces.
					-Apply globbing '*' only.
*/
	line = pre_parse(sh);
	mark_operators(line);
	apply_globbing(&line);
	cmd_processes = ft_strsplit(line, SEMI_COL);
/*
**
building a list of processesed cmds if there's any "&&" or "||" operator.
i.e separate processess of cmds
*/

	while (*cmd_processes)
	{
		if (!(curr = (t_job *)malloc(sizeof(t_job))))
			exit(2);
		curr->next = NULL;
		curr->processes = get_processs_list(*cmd_processes, sh);
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
		cmd_processes++;
	}
	return (head);
}
