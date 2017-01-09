/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsub_end.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/26 10:12:14 by avacher           #+#    #+#             */
/*   Updated: 2015/12/28 12:34:33 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsub_end(char const *s, char c)
{
	int		i;
	int	cpt;
	char	*tmp;

	i = 0;
	cpt = 0;
	tmp = NULL;
	if (s != NULL && ft_strchr())
	{
		tmp = (char *)malloc(sizeof(char) * (len + 1));
		if (tmp != NULL)
		{
			while (s[start + cpt] != '\0' && cpt < len)
			{
				tmp[cpt] = s[start + cpt];
				cpt++;
			}
			tmp[cpt] = '\0';
		}
	}
	return (tmp);
}
