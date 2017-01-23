/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkhaldi <nkhaldi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/09/26 14:37:18 by nkhaldi           #+#    #+#             */
/*   Updated: 2015/11/25 11:31:47 by nkhaldi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		get_nb_word(char const *s, char c)
{
	unsigned int	i;
	unsigned int	nb_word;
	unsigned int	len;

	i = 0;
	nb_word = 0;
	len = 0;
	while (i < (unsigned int)ft_strlen(s))
	{
		if (*(s + i) != c)
			len++;
		else if (*(s + i) == c && len > 0)
		{
			nb_word++;
			len = 0;
		}
		i++;
	}
	if (*(s + i) == '\0' && len > 0)
		nb_word++;
	return (nb_word);
}

char	**new_tab(char const *s, char c)
{
	unsigned int	nb_word;
	unsigned int	i;
	char			**res;

	i = 0;
	nb_word = get_nb_word(s, c);
	if (!(res = (char **)malloc((nb_word + 1) * sizeof(char *))))
		return (NULL);
	return (res);
}

char	**ft_strsplit(char const *s, char c)
{
	char			**res;
	unsigned int	i;
	unsigned int	j;
	unsigned int	len;

	i = 0;
	j = 0;
	len = 0;
	res = new_tab(s, c);
	while (*(s + i))
	{
		if (*(s + i) != c)
			len++;
		else if (*(s + i) == c && len > 0)
		{
			*(res + j) = ft_strsub(s, i - len, len);
			j++;
			len = 0;
		}
		i++;
	}
	if (*(s + i) == '\0' && len > 0)
		*(res + j++) = ft_strsub(s, i - len, len);
	*(res + j) = NULL;
	return (res);
}
