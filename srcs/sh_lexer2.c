#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

#define RA RED_ARG
#define CA CMD_ARG

int				land_managment(t_e_list **tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LAND MANAGMENT ------\n");
	t_e_list		*new;
	t_e_list		*tmp2;
	char			*red_arg;

	red_arg = NULL;
	new = NULL;
	tmp2 = (*tmp)->next;

	if (tmp2 && tmp2->data[0] == '&' && tmp2->next)// && (ft_isstrnum(tmp2->next->data) || (ft_strlen(tmp2->next->data) == 1 && tmp2->next->data[0] == '-')))
	{
		red_arg = ft_properjoin(tmp2->data, tmp2->next->data);
		free(tmp2->data);
		tmp2->data = ft_strdup(red_arg);
		tmp2->type = RA;
		free(tmp2->next->data);
		new = tmp2->next->next;
		free(tmp2->next);
		tmp2->next = new;
		return (TRUE);
	}
	else if ((*tmp)->next && (*tmp)->next->data[0] == '&' && (*tmp)->next->next)
	{
		free((*tmp)->next->data);
		new = (*tmp)->next->next;
		free((*tmp)->next);
		(*tmp)->next = new;
		return (TRUE);
	}
	return (FALSE);
}

// echanger & avec >
// donc echanger tmp avec tmp->next
int				swap_list(t_e_list **tmp)
{
	char	*data1;
	int		type1;

	data1 = ft_strdup((*tmp)->data);
	type1 = (*tmp)->type;

	free((*tmp)->data);
	(*tmp)->data = ft_strdup((*tmp)->next->data);
	(*tmp)->type = (*tmp)->next->type;

	(*tmp)->next->data = ft_strdup(data1);
	(*tmp)->next->type = type1;

	free(data1);

	return (TRUE);
}

int				waka_lexer(t_e_list **tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- WAKA LEXER ------\n");
	t_e_list		*elem;
	char			fd[2];
	char			*red;
	t_e_list		*new;

	red = NULL;
	new = NULL;
	elem = (*tmp)->next;

	if (ft_strchr((*tmp)->data, '&') && (elem->data[0] == '<' || elem->data[0] == '>'))
	{
		(*tmp)->type = RED_FD;
		(*tmp)->next->type = RED;
		swap_list(tmp);
		land_managment(&((*tmp)->next));
	}
	else if (ft_isdigit((elem->data)[0]))
	{
		fd[0] = (elem->data)[0];
		fd[1] = '\0';
		red = ft_strsub(elem->data, 1, ft_strlen(elem->data) - 1);
		new = expr_new(fd);
		new->type = RED_FD;
		free((*tmp)->next->data);
		(*tmp)->next->data = ft_strdup(red);
		new->next = (*tmp)->next->next;
		(*tmp)->next->next = new;
		(*tmp)->next->type = RED;
		land_managment(&((*tmp)->next->next));
		return (0);
	}
	else
	{
		(*tmp)->next->type = RED;
		land_managment((&(*tmp)->next));
		return (0);
	}
	return (-1);
}

int				in_lexer_2(t_e_list **tmp, int boule)
{
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- IN LEXER 2 ------\n");
	while (*tmp && (*tmp)->next)
	{
		// si data == 1 OR .. OR \0 OR & AND data->next == > OR <
		if (ft_strchr((*tmp)->next->data, '<') || ft_strchr((*tmp)->next->data, '>'))
		{
			waka_lexer(&(*tmp));
		}
		else if (boule == 1 && (!ft_strchr(SPECIAL, ((*tmp)->next->data)[0]) &&
					((*tmp)->type == CMD || (*tmp)->type == CA || (*tmp)->type == RA)))
			(*tmp)->next->type = CA;
		else if (((*tmp)->type == RED && (*tmp)->next->type != RED_FD) || (*tmp)->type == RED_FD)
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
	if (DEBUG_LEXER_PARSER == 1)
		printf("------- LEXER 2 ------\n");
	t_e_list		*tmp;
	int				boule;

	tmp = *l_expr;
	boule = 0;

	if (tmp && (ft_strchr(tmp->data, '<') || ft_strchr(tmp->data, '>')))
	{
		waka_lexer(&tmp);
		tmp = tmp->next;
	}
	else if (tmp)
	{
		tmp->type = CMD;
		boule = 1;
	}
	in_lexer_2(&tmp, boule);

	return (0);
}
