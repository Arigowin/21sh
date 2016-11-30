/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolewski <dolewski@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/10 10:45:10 by dolewski          #+#    #+#             */
/*   Updated: 2016/02/13 18:19:44 by dolewski         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		ft_putunbr(char *color, unsigned int n)
{
	if (n >= 10)
		ft_putunbr(color, n / 10);
	ft_putchar_color(color, (n % 10) + '0');
}

void			ft_putnbr_color(char *color, int n)
{
	if (n < 0)
	{
		ft_putchar('-');
		ft_putunbr(color, -n);
	}
	else
		ft_putunbr(color, n);
}
