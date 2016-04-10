/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 20:59:32 by avacher           #+#    #+#             */
/*   Updated: 2015/11/29 13:05:17 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int		cpt;

	cpt = 0;
	if (s != NULL && f != NULL)
	{
		while (s[cpt])
		{
			f(cpt, &s[cpt]);
			cpt++;
		}
	}
}
