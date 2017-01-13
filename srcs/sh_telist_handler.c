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
	new->data = NULL;
	new->type = NONE;
	new->next = NULL;
	if ((new->data = ft_strdup(content)) == NULL)
		return (NULL);
	return (new);
}

int 				expr_pushbk(t_e_list **l_expr, char *data_tmp)
{
	if (DEBUG_LEXER_PARSER == 1)
		ft_putendl_fd("------- EXPR PUSHBK ------", 2);

	t_e_list			*tmp;

	tmp = NULL;
	if (data_tmp == NULL && *data_tmp == '\0')
		return (FALSE);
	if (!(l_expr && *l_expr))
	{
		if ((*l_expr = expr_new(data_tmp)) == NULL)
			return (ERROR);
		return (TRUE);
	}
	tmp = *l_expr;
	while (tmp->next)
		tmp = tmp->next;
	if ((tmp->next = expr_new(data_tmp)) == NULL)
		return (ERROR);
	return (TRUE);
}
