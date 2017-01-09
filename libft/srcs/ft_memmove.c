/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:05:09 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:27:32 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

void			*ft_memmove(void *dst, const void *src, int len)
{
	char			*tmp;

	if (len == 0 || dst == src)
		return (dst);
	tmp = (char *)malloc(sizeof(tmp) * len);
	ft_memcpy(tmp, src, len);
	ft_memcpy(dst, tmp, len);
	free(tmp);
	return ((void *)dst);
}
