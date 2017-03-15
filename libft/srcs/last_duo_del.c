/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_duol_del.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:03:13 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:49:47 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int					last_duol_del(t_duo **lst)
{
	t_duo				*tmp;

	tmp = *lst;
	while (tmp && tmp->next && tmp->next->next)
		tmp = tmp->next;
	if (tmp && tmp->next)
	{
		ft_strdel(&(tmp->next->name));
		ft_strdel(&(tmp->next->value));
		free(tmp->next);
		tmp->next = NULL;
	}
	return (0);
}
