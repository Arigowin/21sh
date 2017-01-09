/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_color_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolewski <dolewski@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 15:47:01 by dolewski          #+#    #+#             */
/*   Updated: 2016/03/07 12:22:01 by dolewski         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void		ft_putchar_color_fd(int fd, char *color, char c)
{
	write(fd, color, ft_strlen(color));
	write(fd, &c, 1);
	write(fd, DEFAULT_COLOR, ft_strlen(DEFAULT_COLOR));
}
