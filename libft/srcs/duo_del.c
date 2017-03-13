/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   duo_del.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/10 10:47:52 by avacher           #+#    #+#             */
/*   Updated: 2016/01/10 12:01:07 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			duo_del(t_duo **lst)
{
	t_duo		*tmp;

	tmp = NULL;
	if (lst  == NULL || *lst == NULL)
		return (-1);
	while ((*lst)->next)
	{
		tmp = (*lst)->next;
		ft_strdel(&(*lst)->name);
		ft_strdel(&(*lst)->value);
		free(*lst);
		*lst = tmp;
	}
	ft_strdel(&(*lst)->name);
	ft_strdel(&(*lst)->value);
	free(*lst);
	*lst = NULL;
	return (0);
}
