/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/27 12:06:56 by avacher           #+#    #+#             */
/*   Updated: 2015/11/27 17:19:12 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_list **alst, void (*del)(void *, size_t))
{
	t_list	*tmp;

	if (alst != NULL)
	{
		while (*alst != NULL)
		{
			tmp = NULL;
			if ((*alst)->next != NULL)
				tmp = (*alst)->next;
			ft_lstdelone(alst, del);
			*alst = tmp;
		}
		*alst = NULL;
	}
}
