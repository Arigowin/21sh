#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

//longeur ok si clear_node (l63) go in retun et si erreur final go in return
int					check_red_arg(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK RED_ARG ------", 2);

	t_node		 		*node;
	t_node				*save;
	t_types				ntype;

	save = *tree;
	node = NULL;
	ntype = ((*l_expr)->hrd_quote >= 2 ? HRD_QUOTE : RED_ARG);
	if ((*l_expr)->type == RED_ARG && ((node = create_node(ntype)) != NULL))
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
			return (sh_error(6, NULL, NULL));
		*tree = node;
		return (TRUE);
	}
	else if ((*l_expr)->type == RED_FD && (*l_expr)->next &&
	(*l_expr)->next->type != RED_FD && ((node = create_node(RED_FD)) != NULL))
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
			return (sh_error(6, NULL, NULL));
		if (!move_in_list(l_expr) || !check_red_arg(l_expr, &(node->right)))
		{
			*tree = save;
			return (FALSE);
		}
		*tree = node;
		return (TRUE);
	}
	return (sh_error(26, (*l_expr)->data, NULL));
}

t_types				fill_red_type(char *data, int *nb_hrd)
{
	t_types				type;

	type = ft_strequ(data, ">") ? RRED : RED;
	type = ft_strequ(data, ">>") ? DRRED : type;
	type = ft_strequ(data, "<") ? LRED : type;
	type = ft_strequ(data, "<<") ? DLRED : type;
	*nb_hrd += (type == DLRED ? 1 : 0);
	return (type);
}

//longeur ok si clear_node (l99) go in retun et si no need pour les 3 der lignes
int					check_red(int *nb_hrd, t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK RED ------", 2);

	t_node				*node;
	t_node				*save;
	t_e_list			*list_save;
	int					red_ret;

	node = NULL;
	save = *tree;
	list_save = *l_expr;
	if ((red_ret = ((*l_expr)->type == RED)) == FALSE)
		return (FALSE);
	if ((*l_expr)->type == RED && (node = create_node(RED)) != NULL
	&& (red_ret = move_in_list(l_expr)) == TRUE
	&& ((red_ret = check_red_arg(l_expr, &(node->right))) == TRUE))
	{
		if ((node->data = ft_strdup(list_save->data)) == NULL)
			return (sh_error(6, NULL, NULL));
		node->type = fill_red_type(list_save->data, nb_hrd);
		if (!move_in_list(l_expr) || check_red(nb_hrd, l_expr, &(node->left)) != TRUE)
			*tree = save;
		*tree = node;
		return (red_ret);
	}
	if (red_ret != NO_PRINT)
		return (sh_error(26, (*l_expr)->data, NULL));
	*tree = save;
	return (red_ret);
}

int					check_arg(int *nb_hrd, t_e_list **l_expr, t_node **tree,
					t_node **right_node)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK ARG ------", 2);

	t_node				*node;
	t_node				*save;

	node = NULL;
	save = *tree;
	if ((*l_expr)->type == CMD_ARG && ((node = create_node(CMD_ARG)) != NULL))
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
		{
			//	clear_node(&node); ?? pas de clear la ?
			return (sh_error(6, NULL, NULL));
		}
		check_next(nb_hrd, l_expr, &save, &(node->right));
		*right_node = node;
		return (TRUE);
	}
	clear_node(&node);
	return (FALSE);
}

int					check_next(int *nb_hrd, t_e_list **l_expr, t_node **tree,
					t_node **right_node)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK NEXT ------", 2);

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
		check_arg(nb_hrd, l_expr, &save, right_node);
		return (TRUE);
	}
	return (FALSE);
}
