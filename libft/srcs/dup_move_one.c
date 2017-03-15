/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_move_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:06:21 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 13:05:49 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int					dup_move_one(char **str)
{
	char				*tmp;

	if ((tmp = ft_strdup(*(str + 1))) == NULL)
		return (-1);
	ft_strdel(str);
	if ((*str = ft_strdup(tmp)) == NULL)
		return (-1);
	ft_strdel(&tmp);
	return (0);
}
