/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_to_tbl.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/26 12:59:59 by avacher           #+#    #+#             */
/*   Updated: 2015/11/29 16:27:42 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char		**lst_to_tbl(t_list *lst)
{
	int			i;
	char		**tbl;

	i = 0;
	if ((tbl = (char **)malloc(sizeof(char *) * (ft_lst_count(lst) + 1)))
			== NULL)
		return (NULL);
	while (lst != NULL)
	{
		if ((tbl[i] = ft_strdup(lst->content)) == NULL)
			return (NULL);
		lst = lst->next;
		i++;
	}
	tbl[i] = NULL;
	return (tbl);
}
