/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/26 16:20:54 by avacher           #+#    #+#             */
/*   Updated: 2015/11/29 15:16:06 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_put_tbl(unsigned int n, int len, int sign, char **str)
{
	int				cpt;

	if (n > 9)
		cpt = ft_put_tbl(n / 10, len + 1, sign, str);
	else
	{
		*str = ft_strnew(len + 1);
		cpt = sign;
	}
	if (*str != NULL)
		(*str)[cpt] = (n % 10) + '0';
	return (cpt + 1);
}

char			*ft_itoa(int n)
{
	char			*str;

	if (n < 0)
	{
		ft_put_tbl(-n, 1, 1, &str);
		if (str != NULL)
			str[0] = '-';
	}
	else
		ft_put_tbl(n, 0, 0, &str);
	return (str);
}
