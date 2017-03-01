#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"
#define RA RED_ARG
#define CA CMD_ARG

static int			waka_land_handler(t_e_list **l_expr, char (*tmp)[], int *i)
{
	if (DEBUG_LEXER == 1)
		ft_putendl_fd("------- WAKA LAND HANDLER ------", 2);

	t_e_list			*new;
	char				*tmp2;

	new = NULL;
	tmp2 = NULL;
	if (ft_strncount((*l_expr)->data, '&') > 1)
		return (FALSE);
	if ((*l_expr)->data[0] == '&' && ft_strchr(((*l_expr)->data) + 1, '<'))
		(*l_expr)->hrd_quote = -42;
	if ((*l_expr)->data[ft_strlen((*l_expr)->data) - 1] == '&')//interieur du  if a mettre dans un fct ?
	{
		(*l_expr)->data[ft_strlen((*l_expr)->data) - 1] = '\0';
		if ((*l_expr)->next != NULL && (ft_isstrnum((*l_expr)->next->data)
		|| ft_strcmp((*l_expr)->next->data, "-") == 0
		|| ft_strcmp((*l_expr)->next->data, "\\-") == 0))
		{
			if ((tmp2 = ft_strjoin("&", (*l_expr)->next->data)) == NULL)
				return (sh_error(FALSE, 6, NULL, NULL));
			ft_strdel(&((*l_expr)->next->data));
			if (((*l_expr)->next->data = ft_strdup(tmp2)) == NULL)
				return (sh_error(FALSE, 6, NULL, NULL));
			ft_strdel(&tmp2);
			(*l_expr)->next->type = RED_ARG;
		}
		else
		{
			if ((new = expr_new("&", 0)) == NULL)
				return (sh_error(FALSE, 6, NULL, NULL));
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
	if (DEBUG_LEXER == 1)
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
	if (DEBUG_LEXER == 1)
		ft_putendl_fd("------- WAKA LEXER ------", 2);

	int					i;
	char				tmp_fd[11];
	char				*data;
	t_e_list			*new;

	i = 0;
	new = NULL;
	ft_bzero(tmp_fd, 11);
	waka_land_handler(l_expr, &tmp_fd, &i);
	if (ft_strchr(WAKA, ((*l_expr)->data)[0]))
	{
		(*l_expr)->type = RED;
		return (TRUE);
	}
	red_fd_copy(l_expr, &tmp_fd, &i);
	//if ((data = ft_strsub((*l_expr)->data, i, ft_strlen((*l_expr)->data) - i)) == NULL)
	if (!(data = ft_strsub((*l_expr)->data, i, ft_strlen((*l_expr)->data) - i))) // ok??
		return (sh_error(FALSE, 6, NULL, NULL));
	ft_strdel(&((*l_expr)->data));
	if (((*l_expr)->data = ft_strdup(data)) == NULL || (tmp_fd[0] == '\0'
	|| (tmp_fd[0] != '\0' && (new = expr_new(tmp_fd, 0)) == NULL)))
	{
		ft_strdel(&data);
		return (sh_error(FALSE, 6, NULL, NULL));
	}
	new->type = RED_FD;
	new->next = (*l_expr)->next;
	(*l_expr)->next = new;
	(*l_expr)->type = RED;
	ft_strdel(&data);
	return (TRUE);
}

int			rightred(int c)
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

	hrd = 0;
	while (l_expr && *l_expr && (*l_expr)->next)
	{
		t = *l_expr;
		hrd = t->next->hrd_quote;
		if (((ft_strchr(t->next->data, '<') || ft_strchr(t->next->data, '>')))
		&& t->next->hrd_quote == 0) // a priori usless mais a tester
		{
			waka_lexer(&(t->next));
			t->next->type = RED;
		}
		else if ((hrd >= 1 || !ft_strchr(SPECIAL, (t->next->data)[0]))
		&& boule == 1 && (t->type == CMD || t->type == CA || t->type == RA))
			t->next->type = CA;
		else if ((/*hrd >= 1 ||*/ !ft_strchr(SPECIAL, (t->next->data)[0]))
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

	t = *l_expr;
	boule = 0;
	if (*l_expr == NULL)
		return (FALSE);
	if (t->hrd_quote < 1 && t && (ft_strchr(t->data, '<')
	|| ft_strchr(t->data, '>')))
		waka_lexer(&t);
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
	type_analyzer(&t, boule);

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
