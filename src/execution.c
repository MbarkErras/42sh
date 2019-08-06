/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 19:13:47 by merras            #+#    #+#             */
/*   Updated: 2019/08/05 17:27:17 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	restore_io(void)
{
	dup2(sh_config_getter(NULL)->stdin, 0);
	dup2(sh_config_getter(NULL)->stdout, 1);
	dup2(sh_config_getter(NULL)->stderr, 2);
}

char    *direct_access_check(char *exec)
{
	if (!access(exec, F_OK))
	{
		if (access(exec, X_OK))
		{
			ft_perror(EXEC_NAME, exec, N_PRM);
			return (NULL);
		}
		if (is_directory(exec))
		{
			ft_perror(EXEC_NAME, NULL, I_DIR);
			return (NULL);
		}
		return (ft_strdup(exec));
	}
	else
		return ((char *)-1);
}

char    *file_lookup(char *file, char **paths)
{
	char                    *exec;
	struct dirent   *dir;
	DIR                             *dird;
	int                             i;

	dird = NULL;
	i = -1;
	while (paths[++i])
	{
		if (!(dird = opendir(paths[i])))
			continue ;
		while ((dir = readdir(dird)))
		{
			if (!ft_strcmp(file, dir->d_name))
			{
				closedir(dird);
				exec = ft_strjoin_free(ft_strjoin(paths[i], "/"),
						dir->d_name, 1);
				return (exec);
			}
		}
		closedir(dird);
	}
	return (NULL);
}

char    *find_exec(char *exec)
{
	char    **paths;
	char    *exec_path;

	paths = NULL;
	if ((int)(exec_path = direct_access_check(exec)) != -1)
		return (exec_path);
	else
	{
		if (read_env("PATH") && ft_strlen(read_env("PATH")))
			paths = ft_strsplit(read_env("PATH"), ':');
		else
		{
			ft_perror(EXEC_NAME, exec, N_EXE);
			return (NULL);
		}
		if (!(exec_path = file_lookup(exec, paths)))
			ft_perror(EXEC_NAME, exec, N_EXE);
		free_char_array(paths);
		return (exec_path);
	}
}

int		execute_command(t_cmd cmd)
{
	char	*exec_path;
	int		child_pid;
	int		exec_exit;

	if (!apply_redirections(cmd.redir))
		return (0);
	if (is_builtin(cmd.arg[0]))
	{
		exec_exit = builtins_dispatcher(&cmd.arg);
		return (exec_exit);
	}
	if (!(exec_path = find_exec(cmd.arg[0])))
	{
		ft_perror(EXEC_NAME, cmd.arg[0], N_EXE);
		return (0);
	}
	if (!(child_pid = fork()))
	{
		exec_exit = execve(exec_path, cmd.arg, env_converter());
		exit(0);
	}
	free(cmd.arg);
	return (child_pid);
}

int		execute_commands(t_cmd *cmd)
{
	int	p[2];
	int fd_pipe = dup(0);
	int child_pid;
	int stat;
	t_cmd	*tmp;

	reset_input_mode();
	while (cmd)
	{
		tmp = cmd->next;
		dup2(fd_pipe, 0);
		close(fd_pipe);
		if (cmd->flag == PIPE || cmd->heredoc)
		{
			if (pipe(p) < 0)
				ft_putendl_fd("Pipe failed.", 2);
			fd_pipe = p[0];
			dup2(p[1], 1);
			close(p[1]);
		}
		else
			dup2(sh_config_getter(NULL)->stdout, 1);
		if (cmd->heredoc)
		{
			ft_putstr(cmd->heredoc);
			free(cmd->heredoc);
			cmd->heredoc = NULL;
			continue ;
		}
		child_pid = execute_command(*cmd);
		if (cmd->flag != PIPE)
			waitpid(child_pid, &stat, 0);
		if ((cmd->flag == AND && stat) || (cmd->flag == OR && !stat))
		{
			free(cmd);
			cmd = tmp;
			tmp = cmd->next;
			free(cmd);
			cmd = tmp;
			continue ;
		}
		free(cmd);
		cmd = tmp;
	}
	restore_io();
	init_terminal();
	return (1);
}

int		execute_command_line(t_commands *commands)
{
	int		last_ret;
	t_commands	*tmp;

	last_ret = 1;
	while (commands)
	{
		tmp = commands->next;
		commands->return_val = execute_commands(commands->chain);
		last_ret = commands->return_val;
		free(commands);
		commands = tmp;
	}
	return (last_ret);
}
