#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"
#define RA RED_ARG

int					manage_hrd_arg(char *red, t_types *ntype, t_e_list **l_expr)
{
	if ((*l_expr)->hrd_quote >= 2 && (ft_strcmp(red, "<<") == 0))
		*ntype = HRD_QUOTE;
	else
		*ntype = RED_ARG;
	if (ft_strcmp(red, "<<") == 0 && ft_strchr(SPECIAL, (*l_expr)->data[0]))
		return (sh_error(FALSE, 26, (*l_expr)->data, NULL));
	return (TRUE);
}

int					check_red_arg(t_e_list **l_expr, t_node **tree, char *red)
{
	t_node				*node;
	t_node				*save;
	t_types				ntype;
	int					ret;

	save = *tree;
	node = NULL;
	ret = manage_hrd_arg(red, &ntype, l_expr);
	if (ret == TRUE && (*l_expr)->type == RA && (node = create_node(ntype)))
	{
		if (fill_leaf(l_expr, &node) == ERROR)
			return (error_clear_node(FALSE, 6, NULL, &node));
		return (parser_ret_fct(TRUE, tree, &node, NULL));
	}
	else if ((*l_expr)->type == RED_FD && (*l_expr)->next &&
	(*l_expr)->next->type != RED_FD && ((node = create_node(RED_FD)) != NULL))
	{
		if (fill_leaf(l_expr, &node) == ERROR)
			return (error_clear_node(FALSE, 6, NULL, &node));
		if ((ret = move_in_list(l_expr)) == TRUE
		&& (ret = check_red_arg(l_expr, &(node->right), red)) >= 0)
			return (parser_ret_fct(TRUE, tree, &node, NULL));
		return (parser_ret_fct(ret, tree, &save, NULL));
	}
	return (error_clear_node(ret, 26, (*l_expr)->data, &node));
}

int					check_red(int *nb_hrd, t_e_list **l_expr, t_node **tree)
{
	t_node				*node;
	t_node				*save;
	t_e_list			*list_save;
	int					ret;

	node = NULL;
	save = *tree;
	list_save = *l_expr;
	if ((ret = ((*l_expr)->type == RED)) == FALSE)
		return (FALSE);
	if ((*l_expr)->type == RED && (*l_expr)->hrd_quote != -42 &&
	(node = create_node(RED)) &&
	(ret = move_in_list(l_expr)) == TRUE &&
	((ret = check_red_arg(l_expr, &(node->right), list_save->data)) == TRUE))
	{
		if (fill_leaf(&list_save, &node) == ERROR)
			return (error_clear_node(FALSE, 6, NULL, &node));
		node->type = fill_red_type(list_save->data, nb_hrd);
		if (!move_in_list(l_expr) ||
		check_red(nb_hrd, l_expr, &(node->left)) != TRUE)
			*tree = save;
		*tree = node;
		return (ret);
	}
	return (error_clear_node(ret, 26, (*l_expr)->data, &node));
}

int					check_arg(int *nb_hrd, t_e_list **l_expr, t_node **tree,
					t_node **right_node)
{
	t_node				*node;
	t_node				*save;
	int					ret;

	node = NULL;
	save = *tree;
	if ((*l_expr)->type == CMD_ARG && ((node = create_node(CMD_ARG)) != NULL))
	{
		if (fill_leaf(l_expr, &node) == ERROR)
		{
			del_tree(&node);
			return (sh_error(FALSE, 6, NULL, NULL));
		}
		ret = check_next(nb_hrd, l_expr, &save, &(node->right));
		*right_node = node;
		return (ret);
	}
	del_tree(&node);
	return (FALSE);
}

int					check_next(int *nb_hrd, t_e_list **l_expr, t_node **tree,
					t_node **right_node)
{
	t_node				*save;
	int					ret;

	ret = 0;
	save = *tree;
	if (move_in_list(l_expr))
	{
		while (save && save->left != NULL)
			save = save->left;
		if ((ret = check_red(nb_hrd, l_expr, &(save->left))) < 0)
			return (ret);
		if ((ret = check_arg(nb_hrd, l_expr, &save, right_node)) >= 0)
			return (TRUE);
		return (ret);
	}
	return (FALSE);
}
