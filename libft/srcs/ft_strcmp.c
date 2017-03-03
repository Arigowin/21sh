/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:05:45 by avacher           #+#    #+#             */
/*   Updated: 2017/03/03 14:43:28 by dolewski         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int						ft_strcmp(const char *s1, const char *s2)
{
	const unsigned char		*tmp_s1;
	const unsigned char		*tmp_s2;

	tmp_s1 = (const unsigned char*)s1;
	tmp_s2 = (const unsigned char*)s2;
	while (*tmp_s1 == *tmp_s2 && *tmp_s1 != '\0')
	{
		tmp_s1++;
		tmp_s2++;
	}
	return (*tmp_s1 - *tmp_s2);
}
