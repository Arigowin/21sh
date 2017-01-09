/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:07:11 by avacher           #+#    #+#             */
/*   Updated: 2015/12/08 21:07:13 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char				*ft_strnstr(const char *s1, const char *s2, int n)
{
	const char			*save_ptr;
	const char			*save_ptr_2;
	int					i;
	int					j;

	i = 0;
	if (*s2 == '\0')
		return ((char*)s1);
	save_ptr_2 = s2;
	while ((*s1) && (i < n))
	{
		j = i++;
		save_ptr = s1;
		while (((*save_ptr == *save_ptr_2) || (*save_ptr_2 == '\0'))
					&& (j <= n))
		{
			if (*save_ptr_2++ == '\0')
				return ((char*)s1);
			save_ptr++;
			j++;
		}
		save_ptr_2 = s2;
		s1++;
	}
	return (NULL);
}
