/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_string_length.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/28 10:34:52 by merras            #+#    #+#             */
/*   Updated: 2019/08/03 16:16:41 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		t_string_length(t_string *head)
{
	int i;

	i = 0;
	while (head)
	{
		head = head->next;
		i++;
	}
	return (i);
}
