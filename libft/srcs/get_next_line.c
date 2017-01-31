/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/28 15:19:23 by avacher           #+#    #+#             */
/*   Updated: 2016/01/06 17:07:48 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "get_next_line.h"

static int		concat_buff(char **e_buff, char **line)
{
	char			*newline;

	newline = ft_strchr(*e_buff, '\n');
	*line = ft_strsub(*e_buff, 0, newline - *e_buff);
	ft_strcpy(*e_buff, newline + 1);
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static char		*e_buff;
	char			buff[GNL_BUFF_SIZE + 1];
	int				read_ret;

	if (line == NULL || fd < 0 || fd > 255)
		return (-1);
	if (e_buff && e_buff[0] && (ft_strchr(e_buff, '\n') != NULL))
		return (concat_buff(&(e_buff), line));
	while ((read_ret = read(fd, buff, GNL_BUFF_SIZE)) > 0)
	{
		buff[read_ret] = '\0';
		if ((e_buff = ft_properjoin(e_buff, buff)) == NULL)
			return (-1);
		if (ft_strchr(e_buff, '\n') != NULL)
			return (concat_buff(&(e_buff), line));
	}
	if (read_ret == -1)
		return (-1);
	if (e_buff && *(e_buff))
	{
		*line = ft_strdup(e_buff);
		ft_strdel(&(e_buff));
		return (1);
	}
	return (0);
}
