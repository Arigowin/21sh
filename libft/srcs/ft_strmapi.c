/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 21:19:40 by avacher           #+#    #+#             */
/*   Updated: 2015/11/29 13:08:29 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strmapi(char const *s, char (*f)(unsigned int, char))
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
				new[cpt] = f(cpt, s[cpt]);
				cpt++;
			}
			new[cpt] = '\0';
		}
	}
	return (new);
}
