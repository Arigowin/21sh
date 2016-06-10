#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

#define RA RED_ARG
#define CA CMD_ARG

int				in_lexer_2(t_e_list **tmp, int boule)
{
	while (*tmp && (*tmp)->next)
	{
		if (ft_strchr("><", ((*tmp)->next->data)[0]))
			(*tmp)->next->type = RED;
		else if (boule == 1 && (!ft_strchr(SPECIAL, ((*tmp)->next->data)[0]) &&
			((*tmp)->type == CMD || (*tmp)->type == CA || (*tmp)->type == RA)))
			(*tmp)->next->type = CA;
		else if ((*tmp)->type == RED)
			(*tmp)->next->type = RA;
		else if (((*tmp)->next->data)[0] == ';')
		{
			(*tmp)->next->type = SEMI;
			boule = 0;
		}
		else if (((*tmp)->next->data)[0] == '|')
		{
			(*tmp)->next->type = PIPE;
			boule = 0;
		}
		else if (boule == 0 && ((ft_strchr(SPECIAL, ((*tmp)->data)[0])
		&& !ft_strchr("><", ((*tmp)->next->data)[0])) || (*tmp)->type == RA))
			(*tmp)->next->type = CMD;
		*tmp = (*tmp)->next;
	}
	return (0);
}

int				lexer_2(t_e_list **l_expr)
{
	t_e_list		*tmp;
	char			boule;

	tmp = *l_expr;
	boule = 0;
	if (tmp && ft_strchr("><", (tmp->data)[0]))
		tmp->type = RED;
	else if (tmp)
	{
		tmp->type = CMD;
		boule = 1;
	}
	in_lexer_2(&tmp, boule);
	return (0);
}
