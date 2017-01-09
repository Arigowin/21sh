/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_int_lst_new.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:03:13 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:49:47 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_intlst		*ft_intlst_new(int data)
{
	t_intlst		*new;

	if ((new = (t_intlst *)malloc(sizeof(t_intlst))) == NULL)
		return (NULL);
	new->data = data;
	new->next = NULL;
	return (new);
}
