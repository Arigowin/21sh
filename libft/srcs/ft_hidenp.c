/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hidenp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 12:05:47 by exam              #+#    #+#             */
/*   Updated: 2016/01/10 12:04:38 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_hidenp(char *hiden, char *str)
{
	int		cpt;
	int		cpt_str;

	cpt = 0;
	cpt_str = 0;
	while (str[cpt_str] != '\0')
	{
		if (str[cpt_str] == hiden[cpt])
			cpt++;
		if (hiden[cpt] == '\0')
			return (1);
		cpt_str++;
	}
	return (0);
}
