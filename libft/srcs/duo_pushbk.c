/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   duo_pushback.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/10 10:47:52 by avacher           #+#    #+#             */
/*   Updated: 2016/01/10 12:01:07 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_duo		*duo_new(char *name, char *value)
{
	t_duo			*new;

	if ((new = (t_duo *)malloc(sizeof(t_duo))) == NULL)
		return (NULL);
	new->name = NULL;
	new->value = NULL;
	new->next = NULL;
	if (name == NULL && value == NULL)
		return (NULL);
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	return (new);
}

int					duo_pushback(t_duo **lst, char *name, char *value)
{
	t_duo			*tmp;

	tmp = *lst;
	if (tmp == NULL)
	{
		*lst = duo_new(name, value);
		return (0);
	}
	while (tmp && tmp->next)
		tmp = tmp->next;
	tmp->next = duo_new(name, value);
	return (0);
}
