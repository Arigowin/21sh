/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_color_fd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dolewski <dolewski@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/06 15:46:59 by dolewski          #+#    #+#             */
/*   Updated: 2016/03/07 12:22:00 by dolewski         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

void		ft_putstr_color_fd(int fd, char *color, char *s)
{
	write(fd, color, ft_strlen(color));
	write(fd, s, ft_strlen(s));
	write(fd, DEFAULT_COLOR, ft_strlen(DEFAULT_COLOR));
}
