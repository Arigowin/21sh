#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"
#define RA RED_ARG
#define CA CMD_ARG

static int			rightred(int c)
{
	if (c == '-' || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

static int			type_analyzer2(int hrd, t_e_list **t, int *bole)
{
	if (DEBUG_LEXER == 1)
		ft_putendl_fd("------- TYPE ANALYZER2 ------", 2);

	if (hrd < 1 && ft_strcmp((*t)->next->data, ";") == 0)
	{
		(*t)->next->type = SEMI;
		*bole = 0;
	}
	else if (hrd < 1 && ((*t)->next->data)[0] == '|')
	{
		(*t)->next->type = (((*t)->next->data)[1] ==
			((*t)->next->data)[0] ? LOGIC_OR : PIPE);
		*bole = 0;
	}
	else if (hrd < 1 && (*t)->type != RED && ((*t)->next->data)[0] ==
	'&' && (!((*t)->next->data)[1] || !rightred((*t)->next->data[1])))
	{
		(*t)->next->type = (((*t)->next->data)[1] == ((*t)->next->data)[0]
			? LOGIC_AND : AMP);
		*bole = 0;
	}
	else if ((*t)->type != RED && ((ft_strchr(SPECIAL, ((*t)->data)[0]) &&
	!ft_strchr("><", ((*t)->next->data)[0])) || (*t)->type == RA) && *bole == 0)
	{
		*bole = 1;
		(*t)->next->type = CMD;
	}
	return (TRUE);
}

static int			type_analyzer(t_e_list **l_expr, int boule)
{
	if (DEBUG_LEXER == 1)
		ft_putendl_fd("------- TYPE ANALYZER ------", 2);

	int					hrd;
	t_e_list			*t;

	while (l_expr && *l_expr && (*l_expr)->next)
	{
		t = *l_expr;
		hrd = t->next->hrd_quote;
		if (((ft_strchr(t->next->data, '<') || ft_strchr(t->next->data, '>')))
		&& t->next->hrd_quote == 0) // a priori usless mais a tester
		{
			if (waka_lexer(&(t->next)) == -4)
				return (-4);
			t->next->type = RED;
		}
		else if ((hrd >= 1 || !ft_strchr(SPECIAL, (t->next->data)[0]))
		&& boule == 1 && (t->type == CMD || t->type == CA || t->type == RA))
			t->next->type = CA;
		else if ((!ft_strchr(SPECIAL, (t->next->data)[0]))
		&& ((t->type == RED && t->next->type != RED_FD) || t->type == RED_FD))
			t->next->type = RA;
		else
			type_analyzer2(hrd, l_expr, &boule);
		*l_expr = (*l_expr)->next;
	}
	return (TRUE);
}

int					lexer(t_e_list **l_expr)
{
	if (DEBUG_LEXER == 1)
		ft_putendl_fd("------- LEXER ------", 2);

	t_e_list			*t;
	int					boule;
	int					ret;

	t = *l_expr;
	boule = 0;
	if (*l_expr == NULL)
		return (FALSE);
	if (t->hrd_quote < 1 && t && (ft_strchr(t->data, '<')
	|| ft_strchr(t->data, '>')))
	{
		if (waka_lexer(&t) == -4)
			return (-4);
	}
	else if (t->hrd_quote < 1 && t && ft_strcmp(t->data, ";") == 0)
		t->type = SEMI;
	else if (t->hrd_quote < 1 && (t->data)[0] == '|')
		t->type = ((t->data)[1] == (t->data)[0] ? LOGIC_OR : PIPE);
	else if (t->hrd_quote < 1 && (t->data)[0] == '&' && (!(t->data)[1]
				|| !rightred(t->data[1])))
		t->type = ((t->data)[1] == (t->data)[0] ? LOGIC_AND : AMP);
	else if (t && t->data && (t->data)[0] != '&' && (t->data)[0] != '|')
	{
		t->type = CMD;
		boule = 1;
	}
	// DEBUG transfomer par
	// return (type_analyzer(&t, boule));
	// // pour norme
	if ((ret = type_analyzer(&t, boule)) != TRUE)
		return (ret);

	// ANTIBUG!!!!!!
	if (DEBUG_LEXER == 1)
	{
		t_e_list *tmp2 = *l_expr;
		while (tmp2)
		{
			printf("[%s-%d-%d] -> ", tmp2->data, tmp2->type, tmp2->hrd_quote);
			tmp2 = tmp2->next;
		}
		printf("\n");
	}
	// fin ANTIBUG !!!!!
	return (TRUE);
}
