/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:04:34 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:17:07 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void			*ft_memccpy(void *dst, const void *src, int c, int n)
{
	char		*tmp_dst;
	char		*tmp_src;

	if (dst && src)
	{
		tmp_dst = (char*)dst;
		tmp_src = (char*)src;
		while (n > 0)
		{
			*tmp_dst = *tmp_src;
			tmp_dst++;
			if (*tmp_src == c)
				return (tmp_dst);
			tmp_src++;
			n--;
		}
	}
	return (NULL);
}
