/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_int_lst_add.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 12:05:47 by exam              #+#    #+#             */
/*   Updated: 2016/01/10 12:04:38 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		ft_intlst_add(t_intlst **alst, int data)
{
	t_intlst	*tmp;

	tmp = *alst;
	if (tmp == NULL)
	{
		*alst = ft_intlst_new(data);
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = ft_intlst_new(data);
}
