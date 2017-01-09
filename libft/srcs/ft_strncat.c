/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:06:47 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:35:38 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char			*ft_strncat(char *s1, const char *s2, int n)
{
	char			*tmp;

	tmp = s1;
	if (!s1 || !s2)
		return (NULL);
	while (*s1)
		s1++;
	while (n > 0 && *s2)
	{
		*s1++ = *s2++;
		n--;
	}
	*s1 = '\0';
	return (tmp);
}
