/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
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

char				*ft_strdup(const char *s1)
{
	int					i;
	char				*rlt;

	i = 0;
	if (s1 == NULL || (rlt = (char*)malloc(ft_strlen(s1) + 1)) == NULL)
		return (NULL);
	while (s1[i])
	{
		rlt[i] = s1[i];
		i++;
	}
	rlt[i] = '\0';
	return (rlt);
}
