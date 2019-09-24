/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 08:35:30 by merras            #+#    #+#             */
/*   Updated: 2019/09/24 12:02:36 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mshell.h"

void	search_renderer(t_read *rd, char *keyword, int flag)
{
	while (rd->pos)
		move_left(rd);
	if (flag != 2)
		tputs(tgetstr("rc", NULL), 1, ft_putchar_int);
	tputs(tgetstr("cd", NULL), 1, ft_putchar_int);
	tputs(tgetstr("sc", NULL), 1, ft_putchar_int);
	ft_putstr("(reverse-i-search)`");
	ft_putstr(keyword);
	ft_putstr("': ");
	ft_putstr(*rd->in);	
}

void	reverse_i_search(t_read *rd)
{
	char	*keyword;
	char	**result;
	char	b[2];
	int		render;
	t_list	*temp;

	b[1] = 0;
	keyword = ft_strnew(0);
	result = rd->in;
	free(*result);
	*result = NULL;
	render = 2;
	search_renderer(rd, keyword, render);
	render = 1;
	while (read(1, b, 1))
	{
		if (b[0] == '\n')
			continue ;
		else if (ft_isprint(b[0]) || b[0] == DEL)
		{
			if (b[0] == DEL)
			{
				if (ft_strlen(keyword) > 0)
					keyword[ft_strlen(keyword) - 2] = 0;
			}
			else
				keyword = ft_strjoin_free(keyword, b, 1);	
			if ((temp = list_find_node(sh_config_getter(NULL)->hist,
							word_search_filter, keyword)))
			{
				free(*result);
				*result = ft_strdup(((t_hist *)temp->content)->value);
				render = 1;
			}
			else
				render = 0;
		}
		else
			continue;
		//submit_result();
		if (render)
			search_renderer(rd, keyword, render);
	}
}
