/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/25 15:19:54 by avacher           #+#    #+#             */
/*   Updated: 2015/12/11 11:33:48 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(int size)
{
	void	*tmp;

	if (size != 0)
	{
		tmp = malloc(size);
		if (tmp != NULL)
			ft_bzero(tmp, size);
		return (tmp);
	}
	return (NULL);
}
