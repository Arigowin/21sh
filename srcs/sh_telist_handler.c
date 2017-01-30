//#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "libft.h"
#include "shell.h"

t_e_list			*expr_new(char *content)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- EXPR NEW ------", 2);

	t_e_list			*new;

	if ((new = (t_e_list *)malloc(sizeof(t_e_list))) == NULL)
		return (NULL);
		/* MSG ret: NULL exit: TRUE msg: "malloc fail"
		 * free: content */
	new->data = NULL;
	new->type = NONE;
	new->next = NULL;
	if ((new->data = ft_strdup(content)) == NULL)
		return (NULL);
		/* MSG ret: NULL exit: TRUE msg: "malloc fail"
		 * free: content + new */
	return (new);
}

int				expr_del(t_e_list **new)
{
	//if (DEBUG_LEXER_PARSER == 1)
	//	ft_putendl_fd("------- EXPR DEL ------", 2);

	t_e_list	*tmp;
	t_e_list	*trash;

	tmp = *new;
	while (tmp)
	{
		trash = tmp;
		tmp = tmp->next;
		ft_strdel(&(trash->data));
		free(trash);
	}
	*new = NULL;
	return (TRUE);
}

int 				expr_pushbk(t_e_list **l_expr, char *data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- EXPR PUSHBK ------", 2);

	t_e_list			*tmp;

	if (data_tmp == NULL || *data_tmp == '\0')
		return (FALSE);
	if (!(l_expr && *l_expr))
	{
		*l_expr = expr_new(data_tmp);
		return (TRUE);
		/*
		if ((*l_expr = expr_new(data_tmp)) == NULL) // return useless
			return (ERROR);
		return (TRUE);
		*/
	}
	tmp = *l_expr;
	while (tmp->next)
		tmp = tmp->next;
	if ((tmp->next = expr_new(data_tmp)) == NULL) // return useless
		return (ERROR);
	return (TRUE);
}
