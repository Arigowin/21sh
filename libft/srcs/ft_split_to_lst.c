/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_to_lst.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/28 18:24:00 by avacher           #+#    #+#             */
/*   Updated: 2015/11/29 20:26:03 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list			*ft_split_to_lst(const char *s, char c)
{
	t_list			*tab;
	int				cpt1;
	int				cpt2;
	char			*tmp;

	cpt1 = 0;
	cpt2 = 0;
	tmp = NULL;
	tab = NULL;
	while (s != NULL && s[cpt1])
	{
		cpt1 = cpt2;
		while (s[cpt1] && cpt2 < ft_strlen(s) && s[cpt2] != c)
			cpt2++;
		tmp = ft_strsub(s, cpt1, cpt2 - cpt1);
		if (tmp != NULL && (cpt2 - cpt1) > 0)
		{
			if (tab == NULL)
				tab = ft_lstnew(tmp);
			else
				ft_lstpushback(&tab, tmp);
		}
		cpt2++;
	}
	return (tab);
}
