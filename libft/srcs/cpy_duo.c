/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy_duo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 17:49:05 by avacher           #+#    #+#             */
/*   Updated: 2016/01/13 19:32:15 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_duo				*cpy_duo(t_duo *duo)
{
	t_duo				*duo_cpy;

	duo_cpy = NULL;
	while (duo)
	{
		if (duo_pushback(&duo_cpy, duo->name, duo->value) == -1)
			return (NULL);
		duo = duo->next;
	}
	return (duo_cpy);
}
