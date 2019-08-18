/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_digitcount.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merras <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 22:41:24 by merras            #+#    #+#             */
/*   Updated: 2019/08/18 11:10:50 by merras           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_digitscount(long long int n)
{
	int	d;
	int s;

	d = 0;
	s = 0;
	if (n == 0)
		return (1);
	else if (n == -2147483648)
		return (11);
	if (n < 0)
	{
		n = n * -1;
		s = 1;
	}
	while (n > 0)
	{
		n = n / 10;
		d++;
	}
	return (d + s);
}
