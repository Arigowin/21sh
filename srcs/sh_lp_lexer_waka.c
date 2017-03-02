#include "shell.h"
#include "libft.h"

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

static int			red_fd_copy(t_e_list **l_expr, char (*tmp)[], int *i) // static ac waka lexer
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

int					waka_lexer(t_e_list **l_expr)
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
