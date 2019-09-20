// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   execution.c                                        :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2019/07/07 19:13:47 by merras            #+#    #+#             */
// /*   Updated: 2019/09/20 01:42:09 by yoyassin         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "mshell.h"

// void	restore_io(void)
// {
// 	dup2(STDIN_DUP, 0);
// 	dup2(STDOUT_DUP, 1);
// 	dup2(STDERR_DUP, 2);
// }

// char    *direct_access_check(char *exec)
// {
// 	if (!access(exec, F_OK))
// 	{
// 		if (access(exec, X_OK))
// 		{
// 			ft_perror(EXEC_NAME, exec, N_PRM);
// 			return (NULL);
// 		}
// 		if (is_directory(exec))
// 		{
// 			ft_perror(EXEC_NAME, NULL, I_DIR);
// 			return (NULL);
// 		}
// 		return (ft_strdup(exec));
// 	}
// 	else
// 		return ((char *)-1);
// }

// char    *file_lookup(char *file, char **paths)
// {
// 	char                    *exec;
// 	struct dirent   *dir;
// 	DIR                             *dird;
// 	int                             i;

// 	dird = NULL;
// 	i = -1;
// 	while (paths[++i])
// 	{
// 		if (!(dird = opendir(paths[i])))
// 			continue ;
// 		while ((dir = readdir(dird)))
// 		{
// 			if (!ft_strcmp(file, dir->d_name))
// 			{
// 				closedir(dird);
// 				exec = ft_strjoin_free(ft_strjoin(paths[i], "/"),
// 						dir->d_name, 1);
// 				return (exec);
// 			}
// 		}
// 		closedir(dird);
// 	}
// 	return (NULL);
// }

// char    *find_exec(char *exec)
// {
// 	char    **paths;
// 	char    *exec_path;

// 	paths = NULL;
// 	if ((int)(exec_path = direct_access_check(exec)) != -1)
// 		return (exec_path);
// 	else
// 	{
// 		if (read_env("PATH") && ft_strlen(read_env("PATH")))
// 			paths = ft_strsplit(read_env("PATH"), ':');
// 		else
// 		{
// 			ft_perror(EXEC_NAME, exec, N_EXE);
// 			return (NULL);
// 		}
// 		if (!(exec_path = file_lookup(exec, paths)))
// 			ft_perror(EXEC_NAME, exec, N_EXE);
// 		free_char_array(paths);
// 		return (exec_path);
// 	}
// }
// /*
//    int		execute_command(t_cmd cmd)
//    {
//    char	*exec_path;
//    int		child_pid;
//    int		exec_exit;

//    if (!apply_redirections(cmd.redir))
//    return (0);
//    if (is_builtin(cmd.arg[0]))
//    {
//    exec_exit = builtins_dispatcher(&cmd.arg);
//    return (exec_exit);
//    }
//    if (!(exec_path = find_exec(cmd.arg[0])))
//    {
//    ft_perror(EXEC_NAME, cmd.arg[0], N_EXE);
//    return (0);
//    }
//    if (!(child_pid = fork()))
//    {
//    exec_exit = execve(exec_path, cmd.arg, env_converter());
//    exit(0);
//    }
//    free(cmd.arg);
//    return (child_pid);
//    }
//    int		execute_commands(t_cmd *cmd)
//    {
//    int	p[2];
//    int fd_pipe = dup(0);
//    int child_pid;
//    int stat;
//    t_cmd	*tmp;

//    reset_input_mode();
//    while (cmd)
//    {
//    dup2(fd_pipe, 0);
//    close(fd_pipe);
//    if (cmd->flag == PIPE || cmd->heredoc)
//    {
//    fd_pipe = p[0];
//    close(p[1]);
//    }
//    else
//    dup2(STDIN_DUP, 1);
//    if (cmd->heredoc)
//    {
//    ft_putstr(cmd->heredoc);
//    free(cmd->heredoc);
//    cmd->heredoc = NULL;
//    continue ;
//    }
//    child_pid = execute_command(*cmd);
//    if (cmd->flag != PIPE)
//    waitpid(child_pid, &stat, 0);
//    if ((cmd->flag == AND && stat) || (cmd->flag == OR && !stat))
//    {
//    free(cmd);
//    cmd = tmp;
//    tmp = cmd->next;
//    free(cmd);
//    cmd = tmp;
//    continue ;
//    }
//    cmd = cmd->next;
//    }
//    restore_io();
//    init_terminal();
//    return (1);
// int	p[2];

// reset_input_mode();
// while (cmd)
// {
// 	//dup stdin with pipe read end
// 	if (cmd->flag == PIPE || cmd->heredoc)
// 	{
// 		if (pipe(p) < 0)
// 			return (ft_perror(EXEC_NAME, NULL, F_PIP));
// 		dup2(p[1], 1);
// 	}
// 	if (cmd->heredoc)
// 	{
// 		ft_putstr(cmd->heredoc);
// 		ft_strdel(&cmd->heredoc);
// 		continue ;
// 	}


// }



// restore_io();
// init_terminal();
// }
// */
// /*
//    int		execute_commands(t_commands *job)
//    {
//    }
//    int		execute_command_line(t_commands *commands)
//    {
//    int		last_ret;
//    t_commands	*tmp;

//    last_ret = 1;
//    while (commands)
//    {
// //tmp = commands->next;
// commands->return_val = execute_commands(commands);
// last_ret = commands->return_val;
// commands = commands->next;
// //free(commands);
// //commands = tmp;
// }
// return (last_ret);
// }*/


// int		execute_process(t_process *process, pid_t gid, int fg)
// {
// 	pid_t	pid;
// 	char	*path;

// 	if (F_GET(sh_config_getter(NULL)->flags, F_INTERACTIVE))
// 	{
// 		pid = getpid();
// 		if (!gid)
// 			gid = pid;
// 		setpgid(pid, gid);
// 		if (fg)
// 			tcsetpgrp(STDIN_DUP, gid);
// 		signal (SIGINT, SIG_DFL);
// 		signal (SIGQUIT, SIG_DFL);
// 		signal (SIGTSTP, SIG_DFL);
// 		signal (SIGTTIN, SIG_DFL);
// 		signal (SIGTTOU, SIG_DFL);
// 		signal (SIGCHLD, SIG_DFL);

// 	}
// 	if (execve(path, process->arg, env_converter()) == -1)
// 		return (ft_perror(EXEC_NAME, NULL, F_EXE));
// 	exit(EXIT_FAILURE);
// }

// int		job_is_stopped(t_process *process)
// {
// 	while (process)
// 	{
// 		if (!F_GET(process->jcflags, F_STOP) && !F_GET(process->jcflags, F_COMP))
// 			return (0);
// 		process = process->next;
// 	}
// 	return (1);
// }

// int		update_job_status(pid_t pid, int status, t_process *process)
// {
// 	if (pid > 0)
// 	{
// 		while (process)
// 		{
// 			if (process->pid == pid)
// 			{
// 				process->status = status;
// 				if (WIFSTOPPED(status))
// 					F_SET(process->jcflags, F_STOP);
// 				else
// 				{
// 					F_SET(process->jcflags, F_COMP);
// 					if (WIFSIGNALED(status))
// 						fprintf(stderr, "%d: Terminated by signal %d.\n",
// 								(int) pid, WTERMSIG(process->status));
// 				}
// 				return (0);
// 			}
// 			process = process->next;
// 		}
// 		ft_perror(EXEC_NAME, NULL, N_CHD);
// 		return (-1);
// 	}
// 	else
// 		return (-1);

// }

// void	wait_for_job(t_job *job)
// {
// 	int		status;
// 	pid_t	pid;

// 	pid = waitpid(WAIT_ANY, &status, WUNTRACED);
// 	while (!update_job_status(pid, status, job->processes)
// 			&& !job_is_stopped(job->processes))
// 		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
// }

// void	put_job_in_foreground(t_job *job)
// {
// 	tcsetpgrp(STDIN_DUP, job->gid);
// 	wait_for_job(job);
// }

// void	put_job_in_background(t_job *job)
// {
// 	tcsetpgrp(STDIN_DUP, job->gid);
// }

// void	job_execution_followup(t_job *job)
// {
// 	if (!F_GET(sh_config_getter(NULL)->flags, F_INTERACTIVE))
// 		wait_for_job(job);
// 	else if (F_GET(job->jcflags, F_BACKGROUND))
// 		put_job_in_background(job);
// 	else
// 		put_job_in_foreground(job);
// }

// // /!\ consider input modes stuff
// int		execute_job(t_job *job)
// {
// 	t_process *process;
// 	pid_t		pid;

// 	process = job->processes;
// 	while (process)
// 	{
// 		//piping here my lady
// 		pid = fork();
// 		if (pid == 0)
// 			execute_process(process, job->gid, F_GET(job->jcflags, F_BACKGROUND));
// 		else if (pid < 0)
// 			exit (ft_perror(EXEC_NAME, NULL, F_FRK));
// 		process->pid = pid;
// 		if (F_GET(sh_config_getter(NULL)->flags, F_INTERACTIVE))
// 		{
// 			if (!job->gid)
// 				job->gid = pid;
// 			setpgid(pid, job->gid);
// 		}
// 		//piping cleanup here my lady
// 		process = process->next;
// 	}
// 	job_execution_followup(job);
// 	return (0);
// }

// int		execute_jobs(t_job *jobs)
// {
// 	while (jobs)
// 	{
// 		if (jobs->next)
// 			execute_job(jobs);
// 		else
// 			return (execute_job(jobs));
// 		jobs = jobs->next;
// 	}
// 	return (0);
// }
