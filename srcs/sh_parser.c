#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				clear_node(t_node **node)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CLEAR NODE ------\n");
	if ((*node) && (*node)->type)
		printf("%d \n", (*node)->type);
	if (node && (*node) && !(*node)->left && !(*node)->right)
	{
		ft_strdel(&((*node)->data));
		(*node)->type = NONE;
		free(*node);
		*node = NULL;
		return (TRUE);
	}
	return (FALSE);
}

int				clear_tree(t_node **tree)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CLEAR TREE ------\n");
//	printf("clear tree -- [%s]\n", (*tree)->data);
	if ((*tree) && (*tree)->left)
		clear_tree(&((*tree)->left));
	if ((*tree) && (*tree)->right)
		clear_tree(&((*tree)->right));
	clear_node(tree);
	return (FALSE);
}

int				parse_error(char *data)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- PARSE ERROR ------\n");
	ft_putstr("21sh: parse error near \"");
	ft_putstr(data);
	ft_putendl("\"");
	return (ERROR);
}

int				move_in_list(t_e_list **l_expr)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- MOVE IN LIST ------\n[%s]\t", (*l_expr)->data);
	if ((*l_expr) && (*l_expr)->next)
	{
		(*l_expr) = (*l_expr)->next;
		printf(" --> [%s]\n", (*l_expr)->data);
		return (TRUE);
	}
	printf("\n");
	return (FALSE);
}

static int		filled_red_arg(t_e_list **l_expr, t_node **node)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- FILLED RED ARG ------\n");
	if (((*node)->data = ft_strdup((*l_expr)->data)) == NULL)
	{
		clear_node(node);
		return (FALSE);
	}
	return (TRUE);
}

int				check_red_arg(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CHECK RED_ARG ------\n[%s]\n", (*l_expr)->data);
	t_node			*node;
	t_node			*save;

	node = NULL;
	save = *tree;
	if ((*l_expr)->type == RED_ARG && ((node = create_node(RED_ARG)) != NULL))
	{
		if (filled_red_arg(l_expr, &node) == FALSE)
			return (FALSE);
		*tree = node;
		return (TRUE);
	}
	else if ((*l_expr)->type == RED_FD && ((node = create_node(RED_FD)) != NULL))
	{
		if (filled_red_arg(l_expr, &node) == FALSE)
			return (FALSE);
		if (!move_in_list(l_expr) || !check_red_arg(l_expr, &(node->right)))
			*tree = save;
		*tree = node;
		return (TRUE);
	}
	ft_putendl("missing name for redirect\n");
	parse_error((*l_expr)->data);
	clear_node(&node);
	return (FALSE);
}

int				check_red(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CHECK RED ------\n[%s]\n", (*l_expr)->data);
	t_node			*node;
	t_node			*save;
	t_e_list		*list_save;

	node = NULL;
	save = *tree;
	list_save = *l_expr;
	if ((*l_expr)->type == RED && (node = create_node(RED)) != NULL
			&& move_in_list(l_expr) && check_red_arg(l_expr, &(node->right)))
	{
		if ((node->data = ft_strdup(list_save->data)) == NULL)
		{
			clear_node(&node);
			return (FALSE);
		}
		node->type = ft_strequ(list_save->data, ">") ? RRED : 0;
		node->type = ft_strequ(list_save->data, ">>") ? DRRED : node->type;
		node->type = ft_strequ(list_save->data, "<") ? LRED : node->type;
		node->type = ft_strequ(list_save->data, "<<") ? DLRED : node->type;
		if (!move_in_list(l_expr) || !check_red(l_expr, &(node->left)))
			*tree = save;
		*tree = node;
		return (TRUE);
	}
	clear_node(&node);
	*tree = save;
	return (FALSE);
}

int				check_arg(t_e_list **l_expr, t_node **tree, t_node **right_node)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CHECK ARG ------\n");
	printf("check arg [%s]\n", (*l_expr)->data);
	t_node			*node;
	t_node			*save;

	node = NULL;
	save = *tree;
	if ((*l_expr)->type == CMD_ARG && ((node = create_node(CMD_ARG)) != NULL))
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
		{
			clear_node(&node);
			return (FALSE);
		}
		//		if (!move_in_list(*l_expr) || !check_arg(l_expr, &(node->right)))
		//			*tree = save;
		check_next(l_expr, &save, &(node->right));
		*right_node = node;
		return (TRUE);
	}
	clear_node(&node);
	return (FALSE);
}

//checker pour le retour de check_red, il faut stopper si pas d'argumer de redirection.... ajouter un code pour si il n'y a pas de red_arg
int				check_next(t_e_list **l_expr, t_node **tree, t_node **right_node)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CHECK C PIPE ------\n");
		printf("------- ADD ENV ------\n");
	printf("check next [%s]\n", (*l_expr)->data);
	t_node			*save;

	save = *tree;
	if (move_in_list(l_expr))
	{
		while (save && save->left != NULL)
			save = save->left;
		check_red(l_expr, &(save->left));
		check_arg(l_expr, &save, right_node);
		return (TRUE);
	}
	return (FALSE);
}

int				check_command(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CHECK COMMAND -----\n[%s]\n", (*l_expr)->data);
	t_node			*save;
	t_node			*node;
	int				red;

	save = *tree;
	node = NULL;
	red = 0;
	if ((node = create_node(CMD)) == NULL)
		return (FALSE);
	if ((red = check_red(l_expr, &(node->left))) == FALSE)
		*tree = save;
	if ((*l_expr)->type == CMD) // && (!red || move_in_list(l_expr)))
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
		{
			clear_node(&node);
			return (FALSE);
		}
		check_next(l_expr, &node, &(node->right));
		//		if (!move_in_list(l_expr) || !check_arg(l_expr, &node, &(node->right)))
		//			*tree = save;
		*tree = node;
		return (TRUE);
	}
	printf("error in check cmd\n");
	parse_error((*l_expr)->data);
	clear_node(&node);
	return (FALSE);
}

int				check_c_pipe(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CHECK C PIPE ------\n");
	t_node			*node;
	t_node			**node_to_give;

	node = NULL;
	if ((node = create_node(PIPE)) == NULL)
		return (FALSE);
	node_to_give = (node->left == NULL ? &(node->left) : &(node->right));
	if (check_command(l_expr, node_to_give))
	{
		if ((*l_expr)->type == PIPE)
		{
			node->data = ft_strdup((*l_expr)->data);
			*tree = node;
			if (!(move_in_list(l_expr) && check_c_pipe(l_expr, &(node->right))))
			{
				printf("error in check cpipe\n");
				parse_error((*l_expr)->data);
				clear_node(&node);
				return (FALSE);
			}
			*tree = node;
			return (TRUE);
		}
		*tree = *node_to_give;
		clear_node(&node);
		return (TRUE);
	}
	printf("error in check cpipe\n");
	parse_error((*l_expr)->data);
	clear_node(&node);
	return (FALSE);
}

int				check_expr(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- CHECK EXPR ------\n");
	t_node			*node;
	t_node			**node_to_give;

	node = NULL;
	if ((node = create_node(EXP)) == NULL)
		return (FALSE);
	node_to_give = (node->left == NULL ? &(node->left) : &(node->right));
	if (check_c_pipe(l_expr, node_to_give))
	{
		if ((*l_expr)->type == SEMI && ft_strlen((*l_expr)->data) != 1)
		{
			printf("error in check expr\n");
			parse_error((*l_expr)->data);
			clear_node(&node);
			return (FALSE);
		}
		if ((*l_expr)->type == SEMI)
		{
			node->data = ft_strdup((*l_expr)->data);
			*tree = node;
			if (!(move_in_list(l_expr) && check_expr(l_expr, &(node->right))))
			{
				printf("error in check expr\n");
				parse_error((*l_expr)->data);
				clear_node(&node);
			}
			*tree = node;
			return (TRUE);
		}
		*tree = *node_to_give;
		clear_node(&node);
		return (TRUE);
	}
	printf("error in check expr - pouet!!!!!\n");
	parse_error((*l_expr)->data);
	clear_node(&node);
	return (FALSE);
}

t_node			*parser(t_e_list **l_expr)
{
	if (DEBUG_TREE_CREATION == 1)
		printf("------- PARSER ------\n");
	t_node			*node;

	if (*l_expr == NULL)
		return (NULL);
	node = NULL;
	t_e_list *tmp = *l_expr;
	while(tmp){printf("[%s -> %d] --> ", (tmp)->data, (tmp)->type); tmp = (tmp)->next;}
	printf("\n");
	if ((check_expr(l_expr, &node)) == FALSE)
		clear_tree(&node);
	return (node);
}
