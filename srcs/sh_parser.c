#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				clear_node(t_node **node)
{
	printf("cleqr node\n");
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
	printf("cleqr tree\n");
	if ((*tree)->left)
		clear_tree(&((*tree)->left));
	if ((*tree)->right)
		clear_tree(&((*tree)->right));
	clear_node(tree);
	return (FALSE);
}

int				parse_error(char *data)
{
	ft_putstr("21sh: parse error near \"");
	ft_putstr(data);
	ft_putendl("\"");
	return (ERROR);
}

int				move_in_list(t_e_list **l_expr)
{
	printf("move in list [%s]\n", (*l_expr)->data);
	if ((*l_expr) && (*l_expr)->next)
	{
		(*l_expr) = (*l_expr)->next;
		printf("move [%s]\n", (*l_expr)->data);
		return (TRUE);
	}
	return (FALSE);
}

int				check_red_arg(t_e_list **l_expr, t_node **tree)
{
	printf("check red arg [%s]\n", (*l_expr)->data);
	t_node			*node;

	node = NULL;
	if ((*l_expr)->type == RED_ARG && ((node = create_node(RED_ARG)) != NULL))
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
		{
			clear_node(&node);
			return (FALSE);
		}
		*tree = node;
		return (TRUE);
	}
	printf("error in check red arg\n");
	parse_error((*l_expr)->data);
	//clear_node(&node);
	return (FALSE);
}

int				check_red(t_e_list **l_expr, t_node **tree)
{
	printf("check red [%s]\n", (*l_expr)->data);
	t_node			*node;
	t_e_list		*save;

	node = NULL;
	save = *l_expr;
	if ((*l_expr)->type == RED && (node = create_node(RED)) != NULL
			&& move_in_list(l_expr) && check_red_arg(l_expr, &(node->right)))
	{
		if ((node->data = ft_strdup(save->data)) == NULL)
		{
			clear_node(&node);
			return (FALSE);
		}
		node->type = ft_strequ(save->data, ">") ? RRED : 0;
		node->type = ft_strequ(save->data, ">>") ? DRRED : node->type;
		node->type = ft_strequ(save->data, "<") ? LRED : node->type;
		node->type = ft_strequ(save->data, "<<") ? DLRED : node->type;
		*tree = node;
		if (!move_in_list(l_expr) || !check_red(l_expr, &(node->left)))
			*l_expr = save;
		printf("YXX\n");
		return (TRUE);
	}
	//clear_node(&node);
	*l_expr = save;
	return (FALSE);
}

int				check_arg(t_e_list **l_expr, t_node **tree)
{
	printf("check arg [%s]\n", (*l_expr)->data);
	t_node			*node;
	t_e_list		*save;

	node = NULL;
	save = *l_expr;
	printf("((%s - %d)))\n", (*l_expr)->data, (*l_expr)->type);
	if ((*l_expr)->type == CMD_ARG && ((node = create_node(CMD_ARG)) != NULL))
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
		{
		//	clear_node(&node);
			return (FALSE);
		}
		if (!move_in_list(l_expr))
			*l_expr = save;
		else
			check_arg(l_expr, &(node->right));
		*tree = node;
		return (TRUE);
	}
	//clear_node(&node);
	return (FALSE);
}

int				check_command(t_e_list **l_expr, t_node **tree)
{
	printf("check command [%s]\n", (*l_expr)->data);
	t_e_list		*save;
	t_node			*node;
	int				red;
	int				arg;

	save = *l_expr;
	node = NULL;
	red = 0;
	if ((node = create_node(CMD)) == NULL)
		return (FALSE);
	if ((red = check_red(l_expr, &(node->left))) == FALSE)
	{
		// inutile on ne bouge pas si il n'y a pas de red
		*l_expr = save;
	}
	printf("check command after check red [%s]%d\n", (*l_expr)->data, red);
	if ((*l_expr)->type == CMD /*(!red  || move_in_list(l_expr))*/)
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
		{
			clear_node(&node);
			return (FALSE);
		}
		if (!move_in_list(l_expr))
			*l_expr = save;
		else
		{
			// apres move peut etre
			//  - cmd arg
			//  - red
			printf("XXXn %s\n", node->data);
			arg = check_arg(l_expr, &(node->right));
			red = check_red(l_expr, &(node->left));
			if (!arg && !red)
				*l_expr = save;
		}
		*tree = node;
		return (TRUE);
	}
	printf("error in check cmd\n");
	parse_error((*l_expr)->data);
	//clear_node(&node);
	return (FALSE);
}

int				check_c_pipe(t_e_list **l_expr, t_node **tree)
{
	printf("check cpipe [%s]\n", (*l_expr)->data);
	t_node			*node;

	node = NULL;
	if ((node = create_node(PIPE)) == NULL)
		return (FALSE);
	if (check_command(l_expr, &node))
	{
		*tree = node;
		return (TRUE);
	}
	printf("error in check cpipe\n");
	parse_error((*l_expr)->data);
	clear_node(&node);
	return (FALSE);
}

int				check_expr(t_e_list **l_expr, t_node **tree)
{
	printf("check expr [%s]\n", (*l_expr)->data);
	t_node			*node;

	node = NULL;
	if ((node = create_node(EXP)) != NULL
			&& check_c_pipe(l_expr, &node->left) == TRUE)
	{
		*tree = node;
		return (TRUE);
	}
	return (FALSE);
}

t_node			*parser(t_e_list **l_expr)
{
	printf("parser [%s]\n", (*l_expr)->data);
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
