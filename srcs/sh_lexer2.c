#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

#define RA RED_ARG
#define CA CMD_ARG

int				waka_lexer(t_e_list **tmp)
{
	t_e_list		*elem;
	char			fd;
	char			*red;
	t_e_list		*new;

	elem = *tmp;
	fd = 0;
	if (ft_isdigit((elem->data)[0]))
	{
		fd = (elem->data)[0];
		red = ft_strsub_end(elem->data, (elem->data)[1]);
		new = expr_new(&fd);
		new->type = RED_FD;
		free((*tmp)->data);
		(*tmp)->data = ft_strdup(red);
		new->next = (*tmp)->next;
		(*tmp)->next = new;
		(*tmp)->type = RED;
		(*tmp) = (*tmp)->next;
		return (0);
	}
	return (-1);
}

int				in_lexer_2(t_e_list **tmp, int boule)
{
	while (*tmp && (*tmp)->next)
	{
		if (ft_strchr((*tmp)->next->data, '<') || ft_strchr((*tmp)->next->data, '>'))
			waka_lexer(tmp);
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
		{
			boule = 1;
			(*tmp)->next->type = CMD;
		}
		*tmp = (*tmp)->next;
	}
	return (0);
}

int				lexer_2(t_e_list **l_expr)
{
	t_e_list		*tmp;
	int				boule;

	tmp = *l_expr;
	boule = 0;
	if (tmp && (ft_strchr(tmp->data, '<') || ft_strchr(tmp->data, '>')))
	{
		printf("lexer2 if %s \n", tmp->data);

		tmp->type = RED;
	}
	else if (tmp)
	{
		tmp->type = CMD;
		boule = 1;
	}
	in_lexer_2(&tmp, boule);
	return (0);
}
