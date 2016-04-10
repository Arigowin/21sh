/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/26 11:08:17 by avacher           #+#    #+#             */
/*   Updated: 2015/11/29 15:02:45 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t	cpt1;
	size_t	cpt2;

	cpt1 = 0;
	if (!s)
		return (NULL);
	cpt2 = ft_strlen(s) - 1;
	while (s[cpt1] == ' ' || s[cpt1] == '\n' || s[cpt1] == '\t')
		cpt1++;
	while ((s[cpt2] == ' ' || s[cpt2] == '\n' || s[cpt2] == '\t')
				&& cpt2 > cpt1)
		cpt2--;
	return (ft_strsub(s, cpt1, (cpt2 - cpt1 + 1)));
}
