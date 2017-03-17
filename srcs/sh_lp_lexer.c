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

static int			waka_search(char *str)
{
	if (str != NULL && (ft_strchr(str, '<') || ft_strchr(str, '>')))
		return (TRUE);
	return (FALSE);
}

static int			type_analyzer2(int hrd, t_e_list **t, int *bole)
{
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
	int					hrd;
	t_e_list			*t;

	while (l_expr && *l_expr && (*l_expr)->next)
	{
		t = *l_expr;
		hrd = t->next->hrd_quote;
		if (waka_search(t->next->data) == TRUE && t->next->hrd_quote == 0)
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
	t_e_list			*t;
	int					boule;

	t = *l_expr;
	boule = 0;
	if (*l_expr == NULL)
		return (FALSE);
	if (t->hrd_quote < 1 && t && waka_search(t->data) == TRUE)
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
	return (type_analyzer(&t, boule));
}
