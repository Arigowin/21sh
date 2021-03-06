/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/08 21:03:13 by avacher           #+#    #+#             */
/*   Updated: 2015/12/12 12:49:47 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			free_tab(char ***tab)
{
	int			i;
	int			len;

	if (!tab || !*tab)
		return (-1);
	i = 0;
	len = tbl_len(*tab);
	while (i < len)
	{
		free((*tab)[i]);
		i++;
	}
	free(*tab);
	return (0);
}
