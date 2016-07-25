#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				parse_error(char *data)
{
	ft_putstr("21sh: parse error near \"");
	ft_putstr(data);
	ft_putendl("\"");
	return (ERROR);
}

int				move_in_list(t_e_list *l_expr)
{
	if (l_expr && l_expr->next)
	{
		l_expr = l_expr->next;
		return (1);
	}
	return (0);
}

int				check_red_arg(t_e_list **l_expr, t_node **tree)
{
	t_node			*node;

	node = NULL;
	if (((node = create_node(NULL, RED_ARG)) != NULL) && (*l_expr)->type == RED_ARG)
	{
		node->type = RED_ARG;
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
			return (FALSE);
		*tree = node;
		return (TRUE);
	}
	parse_error((*l_expr)->data);
	//clear_node();
	return (FALSE);
}

int				check_red(t_e_list **l_expr, t_node **tree)
{
	t_node			*node;
	t_e_list		*save;

	node = NULL;
	save = *l_expr;
	if ((node = create_node(NULL, RED)) != NULL && (*l_expr)->type == RED
		&& move_in_list(*l_expr) && check_red_arg(l_expr, &(node->right)))
	{
		if ((node->data = ft_strdup(save->data)) == NULL)
			return (FALSE);
		node->type = ft_strequ(save->data, ">") ? RRED : 0;
		node->type = ft_strequ(save->data, ">>") ? DRRED : node->type;
		node->type = ft_strequ(save->data, "<") ? LRED : node->type;
		node->type = ft_strequ(save->data, "<<") ? DLRED : node->type;
		*tree = node;
		if (!move_in_list(*l_expr) || !check_red(l_expr, &(node->left)))
			*l_expr = save;
		return (TRUE);
	}
	// fct clear_node();
	*l_expr = save;
	return (FALSE);
}

int				check_arg(t_e_list **l_expr, t_node **tree)
{
	t_node			*node;
	t_e_list		*save;

	node = NULL;
	save = *l_expr;
	if (((node = create_node(NULL, CMD_ARG)) != NULL) && (*l_expr)->type == CMD_ARG)
	{
		node->type = CMD_ARG;
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
			return (FALSE);
		if (!move_in_list(*l_expr) || !check_arg(l_expr, &(node->right)))
			*l_expr = save;
		*tree = node;
		return (TRUE);
	}
	//clear_node();
	return (FALSE);
}

int				check_command(t_e_list **l_expr, t_node **tree)
{
	t_e_list		*save;
	t_node			*node;
	int				red;

	save = *l_expr;
	node = NULL;
	red = 0;
	if ((node = create_node(NULL, CMD)) == NULL)
		return (FALSE);
	if (!(red = check_red(l_expr, &(node->left))))
		*l_expr = save;
	if ((!red || move_in_list(*l_expr)) && (*l_expr)->type == CMD)
	{
		node->type = CMD;
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
			return (FALSE);
		if (!move_in_list(*l_expr) || !check_arg(l_expr, &(node->right)))
			*l_expr = save;
		*tree = node;
		return (TRUE);
	}
	parse_error((*l_expr)->data);
	//clear_node();
	return (FALSE);
}

int				check_c_pipe(t_e_list **l_expr, t_node **tree)
{
	t_node			*node;

	node = NULL;
	if ((node = create_node(NULL, PIPE)) == NULL)
		return (FALSE);
	if (check_command(l_expr, &node))
	{
		*tree = node;
		return (TRUE);
	}
	return (FALSE);
}

int				check_expr(t_e_list **l_expr, t_node **tree)
{
	t_node			*node;

	node = NULL;
	if ((node = create_node(NULL, EXP)) != NULL
			&& check_c_pipe(l_expr, &node->left) == TRUE)
	{
		*tree = node;
		return (TRUE);
	}
	return (FALSE);
}

t_node			*parser(t_e_list **l_expr)
{
	t_node			*node;

	if (*l_expr == NULL)
		return (NULL);
	node = NULL;
	if ((check_expr(l_expr, &node)) == FALSE)
	{
		// clear tree
	}
		return (node);
}
