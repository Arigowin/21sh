/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_ignchar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:06:21 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 13:05:49 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include "libft.h"

char				*ft_strdup_ignchar(const char *s1, char c)
{
	int					i;
	int					j;
	char				*rlt;
	int					str_len;

	i = 0;
	j = 0;
	if (s1 == NULL)
		return (NULL);
	str_len = ft_strlen(s1);
	rlt = (char*)malloc(sizeof(char)
			* ((str_len - ft_strncount((char *)s1, c)) + 1));
	if (!rlt)
		return (NULL);
	while (i < str_len)
	{
		if (s1[i] == c)
			i++;
		rlt[j] = s1[i];
		i++;
		j++;
	}
	rlt[j] = '\0';
	return (rlt);
}
