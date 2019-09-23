/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yoyassin <yoyassin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 22:46:55 by merras            #+#    #+#             */
/*   Updated: 2019/09/23 23:54:11 by yoyassin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

/*
** https://www.gnu.org/software/libc/manual/html_node/Noncanon-Example.html
*/

static void	init_terminal_data(void)
{
	char	*termtype;
	int		success;

	termtype = read_env("TERM");
	printf("%s\n", termtype);
	if (termtype == 0)
	{
		ft_perror(EXEC_NAME, NULL, N_TRM);
		exit(0);
	}
	success = tgetent(0, termtype);
	if (success < 0)
	{
		ft_perror(EXEC_NAME, NULL, A_TRM);
		exit(0);
	}
	if (success == 0)
	{
		ft_perror(EXEC_NAME, termtype, S_TRM);
		exit(0);
	}
}

static void	set_input_mode(void)
{
	struct termios	tattr;

	if (!isatty(STDIN_FILENO))
	{
		ft_perror(EXEC_NAME, NULL, N_TTY);
		exit(EXIT_FAILURE);
	}
	tcgetattr(STDIN_FILENO, &tattr);
	tattr.c_lflag &= ~(ICANON | ECHO);
	tattr.c_cc[VMIN] = 1;
	tattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

void		reset_input_mode()
{
	tcsetattr(STDIN_FILENO, TCSANOW, &sh_config_getter(NULL)->saved_attr);
}

void		init_terminal()
{
	init_terminal_data();
	tcgetattr(STDIN_FILENO, &sh_config_getter(NULL)->saved_attr);
	set_input_mode();
}
