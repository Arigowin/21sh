#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

void			lexer_2(t_e_list **l_expr)
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
//	printf("tototututata0 -- %p\n", tmp->next);
	while (tmp && tmp->next)
	{
		if (boule == 1 && (!ft_strchr(SPECIAL, (tmp->next->data)[0]) &&
			(tmp->type == CMD || tmp->type == CMD_ARG || tmp->type == RED_ARG)))
			tmp->next->type = CMD_ARG;
		else if (tmp->type == RED)
			tmp->next->type = RED_ARG;
		else if ((tmp->next->data)[0] == ';')
		{
			tmp->next->type = SEMI;
			boule = 0;
		}
		else if ((tmp->next->data)[0] == '|')
		{
			tmp->next->type = PIPE;
			boule = 0;
		}
		else if (ft_strchr("><", (tmp->next->data)[0]))
			tmp->next->type = RED;
		else if (boule == 0 && ((ft_strchr(SPECIAL, (tmp->data)[0])
			&& !ft_strchr("><", (tmp->next->data)[0])) || tmp->type == RED_ARG))
			tmp->next->type = CMD;
		else
			printf("tototututata5\n");
		tmp = tmp->next;
	}
}
