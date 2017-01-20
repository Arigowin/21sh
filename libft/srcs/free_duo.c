/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_duo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:03:13 by avacher           #+#    #+#             */
/*   Updated: 2017/01/19 21:11:56 by naymatrice       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int				free_duo(t_duo **tab)
{
	t_duo			*tmp;
	t_duo			*tmp2;

	tmp = *tab;
	while (tmp)
	{
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2->name);
		free(tmp2->value);
		free(tmp2);
	}
	*tab = NULL;
	return (0);
}
