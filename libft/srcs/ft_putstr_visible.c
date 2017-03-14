/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_visible.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 15:03:18 by avacher           #+#    #+#             */
/*   Updated: 2015/12/11 11:26:19 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_visible(const char *s)
{
	int		i;

	i = 0;
	while (s && s[i])
	{
		if (ft_isvisible(s[i]))
			ft_putchar(s[i]);
		i++;
	}
}
