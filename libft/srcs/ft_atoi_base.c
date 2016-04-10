/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avacher <avacher@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 18:19:05 by avacher           #+#    #+#             */
/*   Updated: 2015/12/04 12:41:38 by avacher          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static char		ft_toupper(char c)
{
	if (c >= 'a' && c <= 'f')
		return (c - 32);
	return (c);
}

static int		ft_isnummax(char c, int base)
{
	return ((c >= 'A' && c <= 'F') || (c >= '0' && c < (base + '0')));
}

static int		ft_base_change(char c)
{
	int				i;
	static char		tbl_base[16] = {"0123456789ABCDEF"};

	i = 0;
	while (tbl_base[i] != c)
		i++;
	return (i);
}

int				ft_atoi_base(char *str, int base)
{
	int				nb;
	int				cpt;
	int				sign;

	nb = 0;
	cpt = 0;
	sign = 1;
	while (str[cpt] == '\n' || str[cpt] == '\t' || str[cpt] == '\v'
			|| str[cpt] == '\r' || str[cpt] == '\f' || str[cpt] == ' ')
		cpt++;
	if (str[cpt] == '-')
	{
		sign = -1;
		cpt++;
	}
	else if (str[cpt] == '+')
		cpt++;
	while (str[cpt] != '\0' && ft_isnummax(ft_toupper(str[cpt]), base) != 0)
	{
		nb *= base;
		nb += ft_base_change(ft_toupper(str[cpt]));
		cpt++;
	}
	return (nb * sign);
}
