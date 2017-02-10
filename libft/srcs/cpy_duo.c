#include "libft.h"

t_duo				*cpy_duo(t_duo *duo)
{
	t_duo				*duo_cpy;

	duo_cpy = NULL;
	while (duo)
	{
		if (duo_pushback(&duo_cpy, duo->name, duo->value) == -1)
			return (NULL);
		duo = duo->next;
	}
	return (duo_cpy);
}
