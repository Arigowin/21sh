/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:04:53 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:24:16 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

int						ft_memcmp(const void *s1, const void *s2, int n)
{
	const unsigned char		*tmp;
	const unsigned char		*tmp_2;

	if (n == 0)
		return (0);
	tmp = s1;
	tmp_2 = s2;
	while (n--)
	{
		if (*tmp != *tmp_2)
			return (*tmp - *tmp_2);
		tmp++;
		tmp_2++;
	}
	return (0);
}
