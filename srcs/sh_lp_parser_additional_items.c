#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

static int			filled_red_arg(t_e_list **l_expr, t_node **node)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- FILLED RED ARG ------", 2);

	if (((*node)->data = ft_strdup((*l_expr)->data)) == NULL)
	{
//		clear_node(node); //ok
		return (FALSE);
		/* MSG ret: FALSE exit: FALSE msg: malloc fail*/
		/* free: node */
	}
	return (TRUE);
}

//longeur ok si clear_node (l63) go in retun et si erreur final go in return
int					check_red_arg(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK RED_ARG ------", 2);

	t_node		 		*node;
	t_node				*save;

	save = *tree;
	node = NULL;
	if ((*l_expr)->type == RED_ARG && ((node = create_node(RED_ARG)) != NULL))
	{
		if (filled_red_arg(l_expr, &node) == FALSE)
		{
			//		clear_node(&node);
			return (FALSE);
		}
		/* MSG ret: FALSE exit: FALSE msg: NONE*/
		*tree = node;
		return (TRUE);
	}
	else if ((*l_expr)->type == RED_FD && (*l_expr)->next &&
	(*l_expr)->next->type != RED_FD && ((node = create_node(RED_FD)) != NULL))
	{
		if (filled_red_arg(l_expr, &node) == FALSE)
			return (FALSE);
		/* MSG ret: FALSE exit: FALSE msg: NONE*/
		if (!move_in_list(l_expr) || !check_red_arg(l_expr, &(node->right)))
		{
			*tree = save;
			return (FALSE);
			/* MSG ret: FALSE exit: FALSE msg: NONE*/
			/* free: node */
		}
		*tree = node;
		return (TRUE);
	}
	ft_putendl("missing name for redirect\n");
	parse_error((*l_expr)->data);
//	clear_node(&node);
	return (FALSE);
	/* MSG ret: FALSE exit: FALSE msg: missing name for redirect + (*l_expr)->data*/
	/* free: node */
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
	red_ret = TRUE;
	if ((*l_expr)->type == RED && (node = create_node(RED)) != NULL
			&& move_in_list(l_expr) && ((red_ret = check_red_arg(l_expr, &(node->right))) == TRUE))
	{
		if ((node->data = ft_strdup(list_save->data)) == NULL)
		{
			clear_node(&node);
			return (FALSE);
			/* MSG ret: FALSE exit: FALSE msg: malloc fail*/
			/* free: node */
		}
		node->type = ft_strequ(list_save->data, ">") ? RRED : RED;
		node->type = ft_strequ(list_save->data, ">>") ? DRRED : node->type;
		node->type = ft_strequ(list_save->data, "<") ? LRED : node->type;
		node->type = ft_strequ(list_save->data, "<<") ? DLRED : node->type;
		*nb_hrd += (node->type == DLRED ? 1 : 0);
		if (!move_in_list(l_expr) || check_red(nb_hrd, l_expr, &(node->left)) != TRUE)
			*tree = save;
		*tree = node;
		return (TRUE);
	}
	if (red_ret != TRUE)
	{
		ft_putendl("missing name for redirect\n");
		parse_error((*l_expr)->data);
		clear_node(&node);
		return (FALSE);
		/* MSG ret: FALSE exit: FALSE msg: missing name for redirect + (*l_expr)->data */
		/* free: node */
	}
	clear_node(&node);
	*tree = save;
	return (FALSE);
	/* MSG ret: FALSE exit: FALSE msg: NONE */
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
			//	clear_node(&node);
			return (FALSE);
			/* MSG ret: FALSE exit: FALSE msg: malloc fail*/
			/* free: node */
		}
		check_next(nb_hrd, l_expr, &save, &(node->right));
		*right_node = node;
		return (TRUE);
	}
//	clear_node(&node);
	return (FALSE);
	/* MSG ret: FALSE exit: FALSE msg: NONE*/
}

//checker pour le retour de check_red, il faut stopper si pas d'argumer de redirection.... ajouter un message d'errur spécifique
int					check_next(int *nb_hrd, t_e_list **l_expr, t_node **tree,
					t_node **right_node)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK C PIPE ------", 2);

	t_node				*save;

	save = *tree;
	if (move_in_list(l_expr))
	{
		while (save && save->left != NULL)
			save = save->left;
		if (check_red(nb_hrd, l_expr, &(save->left)) == ERROR)
			return (ERROR);
		/* MSG ret: FALSE exit: FALSE msg: NONE */
		/* free: node */
		check_arg(nb_hrd, l_expr, &save, right_node);
		return (TRUE);
	}
	return (FALSE);
/* MSG ret: FALSE exit: FALSE msg: NONE*/
}
