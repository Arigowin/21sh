/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:03:13 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:49:47 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long		ft_atoi_long(const char *str)
{
	long	rlt;
	int		sign;
	char	*tmp;

	rlt = 0;
	sign = 0;
	tmp = (char *)str;
	while (*tmp == '\t' || *tmp == '\n' || *tmp == '\v' || *tmp == '\f'
			|| *tmp == '\r' || *tmp == ' ')
		tmp++;
	sign = (*tmp == '-') ? -1 : 1;
	tmp = (*tmp == '+' || *tmp == '-') ? tmp + 1 : tmp;
	while (*tmp >= '0' && *tmp <= '9')
	{
		rlt = rlt * 10 + *tmp - 48;
		tmp++;
	}
	return (rlt * sign);
}
