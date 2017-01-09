/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:07:17 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:44:31 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

char		*ft_strrchr(const char *s, int c)
{
	char		*tmp;

	if (!s)
		return (NULL);
	tmp = (char*)s;
	while (*tmp != '\0')
		tmp++;
	while ((tmp != s) && (*tmp != c))
		tmp--;
	if ((tmp == s) && (*s != c))
		return (NULL);
	return (tmp);
}
