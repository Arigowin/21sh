#include "libft.h"

void	ft_intlst_add(t_intlst **alst, int data)
{
	t_intlst	*tmp;

	tmp = *alst;
	if (tmp == NULL)
	{
		*alst = ft_intlst_new(data);
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = ft_intlst_new(data);
}
