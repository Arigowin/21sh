#include "libft.h"

t_intlst      *ft_intlst_new(int data)
{
	t_intlst	*new;

	if ((new = (t_intlst *)malloc(sizeof(t_intlst))) == NULL)
		return (NULL);
	new->data = data;
	new->next = NULL;
	return (new);
}
