/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/07 19:13:47 by merras            #+#    #+#             */
/*   Updated: 2019/09/24 14:54:59 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	restore_io(void)
{
	dup2(STDIN_DUP, 0);
	dup2(STDOUT_DUP, 1);
	dup2(STDERR_DUP, 2);
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

int		update_job_status(pid_t pid, int status, t_process *process)
{
	if (pid > 0)
	{
		while (process)
		{
			if (process->pid == pid)
			{
				process->status = status;
				if (WIFSTOPPED(status))
					F_SET(process->jcflags, F_STOP);
				else
				{
					F_SET(process->jcflags, F_COMP);
					if (WIFSIGNALED(status))
						fprintf(stderr, "%d: Terminated by signal %d.\n",
								(int) pid, WTERMSIG(process->status));
				}
				return (0);
			}
			process = process->next;
		}
		ft_perror(EXEC_NAME, NULL, N_CHD);
		return (-1);
	}
	else
		return (-1);
}

int		job_is_stopped(t_process *process)
{
	while (process)
	{
		if (!F_GET(process->jcflags, F_STOP) &&
				!F_GET(process->jcflags, F_COMP))
			return (0);
		process = process->next;
	}
	return (1);
}

int		job_is_completed(t_process *process)
{
	while (process)
	{
		if (!F_GET(process->jcflags, F_COMP))
			return (0);
		process = process->next;
	}
	return (1);
}

void	wait_for_job(t_job *job)
{
	int		status;
	pid_t	pid;

	DEBUG("waiting\n");
	while (1)
	{
		if (job_is_stopped(job->processes))
			break ;
		pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);
		update_job_status(pid, status, job->processes);
	}
	DEBUG("finished waiting\n");
}

void	put_job_in_foreground(t_job *job, int cont)
{
	tcsetpgrp(STDIN_DUP, job->gid);
	if (cont)
	{
		if (kill (- job->gid, SIGCONT) < 0)
			perror ("kill (SIGCONT)");
	}
	wait_for_job(job);
	tcsetpgrp(STDIN_DUP, sh_config_getter(NULL)->shell_pgid);
}

void	put_job_in_background(t_job *job , int cont)
{
	if (cont)
		if (kill (-job->gid, SIGCONT) < 0)
			perror ("kill (SIGCONT)");
}

int		execute_process(char *path, t_process *process, pid_t gid, int bg)
{
	pid_t	pid;


	if (F_GET(sh_config_getter(NULL)->flags, F_INTERACTIVE))
	{
		pid = getpid();
		if (!gid)
			gid = pid;
		setpgid(pid, gid);
		if (!bg)
			tcsetpgrp(STDIN_DUP, gid);
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
	}
	if (process->arg)
		if (!apply_redirections(process->redir) && execve(path, process->arg, env_converter()) == -1)
			exit(EXIT_FAILURE);
	else if (!apply_redirections(process->redir))
		exit(EXIT_FAILURE);
	
}

/*
 * consider:
 *   check case of monitoring when process path is not found
 *
*/

int		execute_job(t_job *job)
{
	t_process *process;
	char	*path;
	pid_t		pid;
	int in;
	int out;
	int err;
	int p[2];

	in = 0;
	err = 2;
	process = job->processes;
	while (process)
	{
		if (job->flag != BG && process->flag == PIPE)
		{
			if (pipe(p) < 0)
				exit(0);  //*-*//
			out = p[1];
		}
		else
			out = 1;
		if (is_builtin(process->arg[0]))
		{
			process->status = builtins_dispatcher(&process->arg);
			F_SET(process->jcflags, F_COMP);
			process = process->next;
			continue ;
		}
		if (!(path = find_exec(process->arg[0])))
			return (ft_perror(EXEC_NAME, process->arg[0], N_EXE));
		process->jcflags = 0;
		pid = fork();
		if (pid == 0)
		{
			if (in != 0)
			{
				dup2(in, 0);
				//close (in);
			}
			if (out != 1)
			{
				dup2(out, 1);
				//close (out);
			}
			//set err
			execute_process(path, process, job->gid, F_GET(job->jcflags, F_BACKGROUND));
		}
		else if (pid < 0)
			exit (ft_perror(EXEC_NAME, NULL, F_FRK));
		process->pid = pid;
		if (F_GET(sh_config_getter(NULL)->flags, F_INTERACTIVE))
		{
			if (!job->gid) //asdasd
				job->gid = pid;
			setpgid(pid, job->gid);
		}
		if (in != 0)
			close(in);
		if (out != 1)
			close (out);
		in = p[0];
		process = process->next;
	}
	if (!F_GET(sh_config_getter(NULL)->flags, F_INTERACTIVE))
	    wait_for_job (job);
	else if (!F_GET(job->jcflags, F_BACKGROUND))
		put_job_in_foreground (job, 0);
	else
		put_job_in_background (job, 0);
	return (0);
}

int		execute_jobs(t_job *jobs)
{
	int exit;

	while (jobs)
	{
		reset_input_mode();
		exit = execute_job(jobs);
		init_terminal();
		if (!jobs->next)
			return (exit);
		jobs = jobs->next;
	}
	return (0);
}
