/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 21:49:12 by merras            #+#    #+#             */
/*   Updated: 2019/08/05 17:27:19 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSHELL_H
# define MSHELL_H

# include <term.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <pwd.h>
# include <dirent.h>
# include "libft.h"
# include <glob.h> // should be removed
# include <stdio.h> // should be removed
# define EXEC_NAME "minishell"
# define PRMPT(x) x ? "(+)> " : "(-)> "

typedef struct	s_read
{
	t_string		*hist;
	char			**in;
	char			*current;
	char			*cboard;
	struct winsize	term;
	int				flags;
	int				prmpt;
	int				pos;
	int				c;
	int				r;
	char			b[2];
	int				cb_start;
	int				cb_end;
}				t_read;

typedef struct	s_redir
{
	int				type;
	int				src_fd;
	int				dst_fd;
	char			*file;
	struct s_redir	*next;
}				t_redir;

typedef struct	s_cmd
{
	char			**arg;
	int				ret_val;

	/*
	**	job control related declarations
	*/
	pid_t			pid;
	int				jcflags;

	t_redir         *redir;
	int				flag;
	char			*heredoc;
	struct s_cmd	*next;
}				t_cmd;

typedef struct	s_commands
{
	t_cmd				*chain;
	int					return_val;

	/*
	**	if job is meant for background F_BACKGROUND
	**	should be set in jcflag
	*/
	pid_t				gid;
	int					jcflag;

	struct s_commands	*next;
}				t_commands;

typedef struct	s_job
{
	pid_t			leader;
	unsigned int	job_number;
	int				job_state;
}				t_job;

typedef struct	s_shell_config
{
	t_string		*hist;
	t_string		*env;
	t_commands		*commands;
	t_job			jobs;
	char			*in;
	char			*cboard;
	int				flags;

	int				stdin;
	int				stdout;
	int				stderr;

	struct termios	saved_attr;
}				t_shell_config;

void			init_terminal();
void			reset_input_mode();
t_shell_config	*sh_config_getter(t_shell_config *sh_set);
char			**env_converter();
void			exit_cleanup(int exit_status, int err);
void			execution_cleanup();


/*
 ** ERROR HANDLING
 */

int				ft_perror(char *command, char *arg, int err);

/*
 ** FLAGS
 */

# define F_GET(x, f) (x & (1 << f))
# define F_BGET(x, f) (x & f)
# define F_SET(x, f) (x |= (1 << f))
# define F_BSET(x, f) (x |= f)
# define F_UNSET(x, f) (x &= ~(1 << f))
# define F_BUNSET(x, f) (x &= ~f)

# define F_LASTRET 0
# define F_CHILDPROC 1

/*
 ** READING
 */

# define F_RDHALT 10
# define F_HIST 9
# define F_SMODE 3
# define F_ESC 0
# define F_CSI 3

# define ESC 27
# define BRA 91
# define CTRLD 4
# define DEL 127
# define END 70
# define HOME 1 //asdasd
# define MOVE_WORD_RIGHT 23
# define MOVE_WORD_LEFT 5
# define MOVE_UP_LINE 18
# define MOVE_DOWN_LINE 20
# define CB_COPY 'c'
# define CB_CUT 'x'
# define CB_PASTE 'v'

# define WORD_DELIMITER(x) (x == ' ' || x == '\n')
# define ARW(x) (x == 'A' || x == 'B' || x == 'C' || x == 'D')
# define END_HOME(x) (x == END || x == HOME)
# define WORD_MOTION(x) (x == MOVE_WORD_RIGHT || x == MOVE_WORD_LEFT)
# define LINE_MOTION(x) (x == MOVE_UP_LINE || x == MOVE_DOWN_LINE)
# define CTRL_MOTION(x) (WORD_MOTION(x) || LINE_MOTION(x))
# define KEY_MOTION(x) (END_HOME(x) || CTRL_MOTION(x))
# define CUR_MOTION(f, r) ((F_BGET(f, F_CSI) && ARW(r)) || KEY_MOTION(r))
# define CBOARD_OP(x) (x == CB_CUT || x == CB_COPY || x == CB_PASTE)
# define TERMINAL_DELETE(x) (x == CTRLD || x == DEL)

void			read_cline(char *prompt, t_shell_config *sh);
t_read			init_read(char *prompt, t_string *hist, char *cboard,
		char **in);
t_read			*rd_config_getter(t_read *rd_set);
void			clipboard_copy_cut(t_read *rd);
void			terminal_delete_dispatcher(t_read *rd);
void			cursor_move_dispatcher(t_read *rd);
void			cboard_dispatcher(t_read *rd);

void			terminal_insert(t_read *rd, char *insert);
void			terminal_delete(t_read *rd, int length);
void			terminal_delete_forward(t_read *rd);

void			move_word(t_read *rd);
void			move_line(t_read *rd);
void			move_character(t_read *rd);
void			move_left(t_read *rd);
void			move_right(t_read *rd);
void			end_home(t_read *rd);
void			move_left_wrap(t_read *rd);
void			get_left(t_read *rd);
void			free_and_init(char **ptr);

char			*delete_chars(char *str, int start, int size);

/*
 ** ERROR HANDLING
 */

# define N_XST 0
# define N_PRM 1
# define N_DIR 2
# define X_ARG 3
# define N_PWD 4
# define N_ARG 5
# define N_CHN 6
# define N_EXE 7
# define N_TRM 8
# define A_TRM 9
# define S_TRM 10
# define N_TTY 11
# define F_EXE 12
# define I_DIR 13
# define N_ENV 14

# define N_XST_T ": no such file or directory: "
# define N_PRM_T ": permission denied: "
# define N_DIR_T ": not a directory: "
# define X_ARG_T ": too many arguments"
# define N_PWD_T ": string not in pwd: "
# define N_ARG_T ": too few arguments"
# define N_CHN_T ": variable name must contain alphanumeric characters"
# define N_EXE_T ": command not found: "
# define N_TRM_T ": specify a terminal type with setenv TERM <yourtype>"
# define A_TRM_T ": could not access the termcap data base"
# define S_TRM_T ": terminal type is not defined: "
# define N_TTY_T ": not a terminal"
# define F_EXE_T ": execution failed"
# define I_DIR_T ": executable is a directory"
# define N_ENV_T ": correspondant environement variable is not set: "

/*
 ** PARSING
 */


# define SEMI_COL -1
# define BLANK -2
# define PIPE -3
# define AND -4
# define OR -5
# define BOTH_FDS -6
# define CLOSE_FD -7
# define HEREDOC -8
# define QUOTE -9
# define D_QUOTE -10
# define DOLLAR_SIGN -109

# define F_BACKGROUND

t_commands		*parse(t_shell_config *sh);
void			apply_expansions(char **args);
int				apply_glob_expansion(char *gl_pattern, char **args);
int				execute_command_line(t_commands *commands);
int				apply_redirections(t_redir *redir);
void			redirections_cleanup(t_redir *redir);

char			*ft_fstrjoin(char *s1, char *s2);
int				ft_strpos(char *s1, char *s2);

/*
** EXPANSIONS
*/

void			expand(t_shell_config *sh);
int				is_delimited(char *string, int c, char d);
void			character_delete(char **string, char to_delete);
void			character_replace(char *string, char find, char replace);
char			*parameter_replace(char *raw, int dollar_pos);

/*
** BUILTINS
*/

char			*read_env(char *key);
void			update_env(char *key, char *value, int flag);
int				is_builtin(char *exec);
int				builtins_dispatcher(char ***arg);
int				cd_dispatcher(char ***in, char *arg, int f);
char			*string_replace(char *src, char *find, char *replace);

int				cd_home(char ***in);
int				cd_replace(char ***in, char *old);
int				cd_old(char ***in);

int				b_env_updater(char **in);

/*
** EXECUTION
*/

int				b_echo(char **arg);
int				b_cd(char ***in);
int				b_senv(char **in, int p);
int				b_env(char **in);
void			b_exit(char **in);

/*
** JOB CONTROL
*/

# define F_CP_STOPPED
# define F_CP_COMPLETED

#endif





