#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"
#define RA RED_ARG
#define CA CMD_ARG

static int			waka_land_handler(t_e_list **l_expr, char (*tmp)[], int *i)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- WAKA LAND HANDLER ------", 2);

	t_e_list			*new;

	new = NULL;
	if (ft_strncount((*l_expr)->data, '&') > 1)
		return (FALSE);
	if ((*l_expr)->data[ft_strlen((*l_expr)->data) - 1] == '&')
	{
		(*l_expr)->data[ft_strlen((*l_expr)->data) - 1] = '\0';
		if (ft_isstrnum((*l_expr)->next->data)
		|| ft_strcmp((*l_expr)->next->data, "-") == 0)
		{
			str_addleft((*l_expr)->next->data, '&');
			(*l_expr)->next->type = RED_ARG;
		}
		else
		{
			if ((new = expr_new("&")) == NULL)
				return (ERROR);
				/* MSG ret: ERROR exit: TRUE msg: "malloc fail"
				 * free:lexpr  */
			new->type = RED_FD;
			new->next = (*l_expr)->next;
			(*l_expr)->next = new;
		}
	}
	if ((*l_expr)->data[0] == '&')
		(*tmp)[(*i)++] = '&';
	return (TRUE);
}

static int			red_fd_copy(t_e_list **l_expr, char (*tmp)[], int *i)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- RED FD COPY ------", 2);

	while (*i < 11 && ft_isdigit(((*l_expr)->data)[*i]))
	{
		(*tmp)[*i] = ((*l_expr)->data)[*i];
		(*i)++;
	}
	return (TRUE);
}

static int			waka_lexer(t_e_list **l_expr)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- WAKA LEXER ------", 2);

	int					i;
	char				tmp[11];
	char				*tmp2;
	t_e_list			*new;

	i = 0;
	new = NULL;
	ft_bzero(tmp, 11);
	if (waka_land_handler(l_expr, &tmp, &i) == ERROR)//return useless
		return (ERROR);
	if (ft_strchr(WAKA, ((*l_expr)->data)[0]))
		return (TRUE);
	red_fd_copy(l_expr, &tmp, &i);
	if ((tmp2 = ft_strsub((*l_expr)->data, i, ft_strlen((*l_expr)->data) - i))
	== NULL)
		return (ERROR);
	/* MSG ret: ERROR exit: TRUE msg: "malloc fail"
		 * free: lexpr  */
	ft_strdel(&((*l_expr)->data));
	if (((*l_expr)->data = ft_strdup(tmp2)) == NULL
	|| (tmp[0] == '\0' || (tmp[0] != '\0' && (new = expr_new(tmp)) == NULL)))
	{
		ft_strdel(&tmp2);
		return (ERROR);
	}
	/* MSG ret: ERROR exit: TRUE msg: "malloc fail"
		 * free: lexpr  */
	new->type = RED_FD;
	new->next = (*l_expr)->next;
	(*l_expr)->next = new;
	ft_strdel(&tmp2);
	return (TRUE);
}

static int			type_analyzer2(t_e_list **l_expr, int *boule)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- TYPE ANALYZER2 ------", 2);

	if (l_expr && *l_expr) // ajout naima
	{
		if (((*l_expr)->next->data)[0] == ';')
		{
			(*l_expr)->next->type = SEMI;
			*boule = 0;
		}
		else if (((*l_expr)->next->data)[0] == '|') // TOTO
		{
			(*l_expr)->next->type = PIPE;
			*boule = 0;
		}
		else if (*boule == 0 && ((ft_strchr(SPECIAL, ((*l_expr)->data)[0])
		 && !ft_strchr("><", ((*l_expr)->next->data)[0])) || (*l_expr)->type == RA))
		{
			*boule = 1;
			(*l_expr)->next->type = CMD;
		}
	}
	return (TRUE);
}

static int			type_analyzer(t_e_list **l_expr, int boule)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- TYPE ANALYZER ------", 2);

	while (l_expr && *l_expr && (*l_expr)->next)
	{
		if (ft_strchr((*l_expr)->next->data, '<')
		|| ft_strchr((*l_expr)->next->data, '>'))
		{
			if (waka_lexer(&((*l_expr)->next)) == ERROR)//return useless
				return (ERROR);
			(*l_expr)->next->type = RED;
		}
		else if (boule == 1 && (!ft_strchr(SPECIAL, ((*l_expr)->next->data)[0])
		&& ((*l_expr)->type == CMD || (*l_expr)->type == CA
		|| (*l_expr)->type == RA)))
			(*l_expr)->next->type = CA;
		else if (!ft_strchr(SPECIAL, ((*l_expr)->next->data)[0])
		&& (((*l_expr)->type == RED && (*l_expr)->next->type != RED_FD)
		|| (*l_expr)->type == RED_FD))
			(*l_expr)->next->type = RA;
		else
			type_analyzer2(l_expr, &boule);
		*l_expr = (*l_expr)->next;
	}
	return (TRUE);
}

int					lexer(t_e_list **l_expr)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- LEXER 2 ------", 2);

	t_e_list			*tmp;
	int					boule;

	tmp = *l_expr;
	boule = 0;
	if (tmp && (ft_strchr(tmp->data, '<') || ft_strchr(tmp->data, '>')))
	{
		waka_lexer(&tmp);
		tmp->type = RED;
	}
	else if (tmp && ft_strcmp(tmp->data, ";") == 0)
		tmp->type = SEMI;
	else if (tmp)
	{
		tmp->type = CMD;
		boule = 1;
	}
	type_analyzer(&tmp, boule);

	// ANTIBUG!!!!!!
	if (DEBUG_LEXER_PARSER == 1)
	{
		t_e_list *tmp2 = *l_expr;
		while (tmp2)
		{
			printf("[%s-%d] -> ", tmp2->data, tmp2->type);
			tmp2 = tmp2->next;
		}
		ft_putendl("");
	}
	// fin ANTIBUG !!!!!
	return (TRUE);
}
