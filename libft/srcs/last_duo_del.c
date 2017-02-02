#include "libft.h"

int					last_duol_del(t_duo **lst)
{
	t_duo				*tmp;

	tmp = *lst;
	while (tmp && tmp->next && tmp->next->next)
		tmp = tmp->next;
	if (tmp && tmp->next)
	{
		ft_strdel(&(tmp->next->name));
		ft_strdel(&(tmp->next->value));
		free(tmp->next);
		tmp->next = NULL;
	}
	return (0);
}
