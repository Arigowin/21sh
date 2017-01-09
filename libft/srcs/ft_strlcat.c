/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:06:32 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:33:50 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

int			ft_strlcat(char *dst, const char *src, int size)
{
	int			len_dst;
	int			len_src;

	len_dst = ft_strlen(dst);
	len_src = ft_strlen(src);
	if (size < len_dst)
		return (len_src + size);
	while (*dst != '\0' && size--)
		dst++;
	while (size-- > 1 && *src != '\0')
		*dst++ = *src++;
	*dst = '\0';
	return (len_dst + len_src);
}
