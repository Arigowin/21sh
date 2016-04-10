/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:04:44 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:21:06 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

void				*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	const char		*tmp;

	i = 0;
	if (s != NULL)
	{
		tmp = s;
		while (i < n)
		{
			if (ft_memcmp(tmp, &c, 1) == 0)
				return ((void*)tmp);
			i++;
			tmp++;
		}
	}
	return (NULL);
}
