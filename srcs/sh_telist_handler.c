#include <stdlib.h>
#include <limits.h>
#include "libft.h"
#include "shell.h"

t_e_list			*expr_new(char *content, int hrd)
{
	t_e_list			*new;

	if ((new = (t_e_list *)malloc(sizeof(t_e_list))) == NULL)
	{
		ft_strdel(&content);
		sh_error(FALSE, 6, NULL, NULL);
	}
	new->data = NULL;
	new->type = NONE;
	new->hrd_quote = hrd;
	new->next = NULL;
	if ((new->data = ft_strdup(content)) == NULL)
	{
		free(new);
		ft_strdel(&content);
		sh_error(FALSE, 6, NULL, NULL);
	}
	return (new);
}

int					expr_del(t_e_list **lst)
{
	t_e_list			*tmp;

	tmp = NULL;
	while (lst && *lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		ft_strdel(&(tmp->data));
		free(tmp);
	}
	tmp = NULL;
	*lst = NULL;
	return (TRUE);
}

int					expr_pushbk(t_e_list **l_expr, char *data_tmp, int hrd)
{
	t_e_list			*tmp;

	if (data_tmp == NULL || *data_tmp == '\0')
		return (FALSE);
	if (!(*l_expr))
		*l_expr = expr_new(data_tmp, hrd);
	else
	{
		tmp = *l_expr;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = expr_new(data_tmp, hrd);
	}
	return (TRUE);
}
