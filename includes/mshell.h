/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mshell.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 21:49:12 by merras            #+#    #+#             */
/*   Updated: 2019/09/22 15:03:28 by mmostafa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSHELL_H
# define MSHELL_H

#  include <term.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <pwd.h>
# include <time.h>
# include <dirent.h>
# include "libft.h"
# include "simplist.h"
# include <glob.h> // should be removed
# include <stdio.h> // should be removed
# define EXEC_NAME "minishell"
# define PRMPT(x) x ? "(+)> " : "(-)> "

typedef struct	s_read
{
	t_list			*hist;
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
	t_list			*editing_history_node; //if !NULL then a history entry is being edited
}				t_read;

typedef struct	s_redir
{
	int				type;
	int				src_fd;
	int				dst_fd;
	char			*file;
	struct s_redir	*next;
}				t_redir;

typedef struct	s_process
{
	char			**arg;
	int				ret_val;
	/*
	**	job control related declarations
	*/
	pid_t			pid;
	int				jcflags;
	int				status;
	// i/o channels should be initiated to defaults during list creation
	int				stdin;
	int				stdout;
	int				stderr;

	t_redir         	*redir;
	int					flag;
	char				*heredoc;
	int					heredoc_fd;
	struct s_process	*next;
}				t_process;

typedef struct	s_pr_group
{
	t_process			*processes;
	int					return_val;
	int					flag;
/*
**	if job is meant for background F_BACKGROUND
**	should be set in jcflags
*/
	// pid_t				gid; //should be initialized to 0
	// int					jcflags; //should be initialized to 0
								//F_BACKGROUND should be set if
								//the job is meant for background

	struct s_pr_group	*next;
}				t_pr_group;

typedef	struct		s_job
{
	t_pr_group		*proc_gr;
	pid_t			gid;
	int				jcflags;
	struct s_job	*next;
}					t_job;


typedef struct	s_hist
{
	char			*value;
	time_t			date;
	char			*fever;
	int			flags;
}				t_hist;

typedef struct	s_variable
{
	char	*value;
	int		flag; //flag to indicate if variable is exported or not
}				t_variable;

typedef struct	s_shell_config
{
	t_read			rd;
	t_list			*hist;
//	t_string		*env;
	t_list			*variables;
	t_job		*jobs;
	char			*in;
	char			*cboard;
	int				flags;

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
t_read			init_read(char *prompt, t_list *hist, char *cboard,
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
void			get_right(t_read *rd);
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
# define F_PIP 15
# define F_FRK 16
# define N_CHD 17
# define H_PRM 18
# define B_HFL 19



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
# define F_PIP_T ": pipe failed"
# define F_FRK_T ": fork failed"
# define N_CHD_T "no child processes"
# define H_PRM_T "not valid range"
# define B_HFL_T "error opening history file"

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
# define OUT_RED_OP -11
# define IN_RED_OP -12
# define APP_OUT_RED_OP -13
# define HEREDOC_OP -14
# define FD_AGGR -15
# define UQ_ESCAPE -16
# define Q_ESCAPE -17
# define DOLLAR -18
# define TILDE -19
# define BG -20
# define IS_REDIRECTION str[i] == OUT_RED_OP || str[i] == IN_RED_OP || str[i] == APP_OUT_RED_OP
# define _OR ||
# define _AND &&
# define _EQ ==
# define _NEQ !=
# define TWO_C_OPS(i, _O, C) line[i] C AND _O \
								line[i] C OR _O \
								line[i] C APP_OUT_RED_OP _O \
								line[i] C HEREDOC_OP
# define ONE_C_OPS(i, _O, C) line[i] C SEMI_COL _O \
								line[i] C PIPE _O \
								line[i] C OUT_RED_OP _O \
								line[i] C IN_RED_OP _O\
								line[i] C BG
# define IS_OPERATOR(i, _O, C) TWO_C_OPS(i, _O, C) _O ONE_C_OPS(i, _O, C)
# define IS_REDIR_OP(i, _O, C) line[i] C OUT_RED_OP _O line[i] C IN_RED_OP \
						_O line[i] C APP_OUT_RED_OP _O line[i] C HEREDOC_OP
# define LEFT_OPR 0b00000001
# define RIGHT_OPR 0b00000010
# define APPEND(type) append((void *)head, (void *)&curr, (void *)tail, type)
# define SETFLAG(type) set_flag(curr, token[j], type)

char			*pre_parse(t_shell_config *sh);
t_job			*parse(t_shell_config *sh);
void			dquotes_checker(char **line, char *dq, int *i, int *j);
void			squotes_checker(char **line, char *q, int *i);
int				check_syntax_errors(char *line);
t_job			*get_jobs(char **cmd_chain);
t_pr_group		*get_parse_list(char *cmd_chain);
t_process		*get_process_list(char *cmd_chain);
char			*check_redirections(char *str, t_process *cmd);
char			escape_char(char c);
int				get_redir_fds(t_redir *curr, char *str, int *i);
void			get_redir_file(t_redir *curr, char *str, int *i);
char			*get_heredoc(char *str, int *i, int *hd_fd);
void			apply_globbing(char **line);
void			apply_expansions(char **args);
int				apply_glob_expansion(char *gl_pattern, char **args);
// int				execute_command_line(t_job *commands);
///
///
int				apply_redirections(t_redir *redir);
void			redirections_cleanup(t_redir *redir);

char			*ft_fstrjoin(char *s1, char *s2);
int				ft_strpos(char *s1, char *s2);
int				is_not_blank(char *line, int j, int i);

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

# define STDIN_DUP 419
# define STDOUT_DUP 420
# define STDERR_DUP 421

int				b_echo(char **arg);
int				b_cd(char ***in);
int				b_senv(char **in, int p);
int				b_env(char **in);
void			b_exit(char **in);

/*
** JOB CONTROL
*/

# define F_BACKGROUND 0
# define F_INTERACTIVE 0
# define F_STOP 0
# define F_COMP 1

// int				execute_jobs(t_job *jobs);
// int				execute_job(t_job *job);
// int				execute_process(t_process *process, pid_t gid, int fg);

/*
** HISTORY
*/

# define F_CHANGED 0
# define F_NEW 1

# define POSI(x, s) x < 0 ? size - x : x

t_hist	*t_hist_construct(t_hist entry);
void	ack_history_change(t_list *node);
void	read_history_resetting();
int		b_history(char **in);
int		print_history();
void    hist_node_del(void *node);
int     history_delete(char **in);
int     history_write(char **in, char c);
int     history_read(char **in, char c);
void	setter(char *set, char *elements);
char	*file_reader(int fd);

/*
 * ** LOCAL VARIABLES
 */
int		ft_export(char **arg);
int		ft_set();
//int		ft_unset(char **arg);
#endif
