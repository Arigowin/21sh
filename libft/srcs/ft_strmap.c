/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 21:06:36 by avacher           #+#    #+#             */
/*   Updated: 2015/11/29 13:06:27 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strmap(char const *s, char (*f)(char))
{
	int			cpt;
	char		*new;

	cpt = 0;
	new = NULL;
	if (s != NULL && f != NULL)
	{
		new = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
		if (new != NULL)
		{
			while (s[cpt])
			{
				new[cpt] = f(s[cpt]);
				cpt++;
			}
			new[cpt] = '\0';
		}
	}
	return (new);
}
