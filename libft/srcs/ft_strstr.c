/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:07:24 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:45:02 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char			*ft_strstr(const char *s1, const char *s2)
{
	const char	*tmp;
	const char	*tmp_2;

	tmp_2 = s2;
	if (*s2 == '\0')
		return ((char*)s1);
	while (*s1 != '\0')
	{
		tmp = s1;
		while (*tmp == *tmp_2)
		{
			tmp++;
			tmp_2++;
			if (*tmp_2 == '\0')
				return ((char*)s1);
		}
		tmp_2 = s2;
		s1++;
	}
	return (NULL);
}
