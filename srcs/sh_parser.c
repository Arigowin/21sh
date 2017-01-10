#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

// fonctionnalité a ajouter dans la/les fonctions d'erreur générales
int					parse_error(char *data)
{
	if (DEBUG_PARSER == 1)
		printf("------- PARSE ERROR ------\n");

	ft_putstr("21sh: parse error near \"");
	ft_putstr(data);
	ft_putendl("\"");
	return (ERROR);
}

int					move_in_list(t_e_list **l_expr)
{
	if (DEBUG_PARSER == 1)
		printf("------- MOVE IN LIST ------\n[%s]\n", (*l_expr)->data);

	if ((*l_expr) && (*l_expr)->next)
	{
		(*l_expr) = (*l_expr)->next;
		return (TRUE);
	}
	return (FALSE);
}


//longeur ok si clear_node (l183) go in retun et si erreur final go in return
int					check_command(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_PARSER == 1)
		printf("------- CHECK COMMAND -----\n[%s]\n", (*l_expr)->data);

	t_node				*save;
	t_node				*node;
	int					red;

	save = *tree;
	red = 0;
	if ((node = create_node(CMD)) == NULL)
		return (FALSE);
	if ((red = check_red(l_expr, &(node->left))) != TRUE)
		*tree = save;
	//printf("red = (%d\n",red);
	if ((*l_expr)->type == CMD) // && (!red || move_in_list(l_expr)))
	{
		if ((node->data = ft_strdup_ignchar((*l_expr)->data, '\\')) == NULL)
		{
			clear_node(&node);
			return (FALSE);
		}
		check_next(l_expr, &node, &(node->right));
		*tree = node;
		return (TRUE);
	}
	if (red == TRUE && (*l_expr)->type != CMD)
	{
		*tree = node->left;
		return (TRUE);
	}
	printf("error in check cmd\n");
	parse_error((*l_expr)->data);
	clear_node(&node);
	return (FALSE);
}

//longeur ok si erreur (l220) go in retun et si erreur final go in return
int					check_c_pipe(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_PARSER == 1)
		printf("------- CHECK C PIPE ------\n");

	t_node				*node;
	t_node				**node_to_give;

	node = NULL;
	if ((node = create_node(PIPE)) == NULL)
		return (FALSE);
	node_to_give = (node->left == NULL ? &(node->left) : &(node->right));
	if (check_command(l_expr, node_to_give))
	{
		if ((*l_expr)->type == PIPE)
		{
			node->data = ft_strdup_ignchar((*l_expr)->data, '\\');
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

//longeur ok si erreur (l258) go in retun et si erreur final go in return
int					check_expr(t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_PARSER == 1)
		printf("------- CHECK EXPR ------\n");

	t_node				*node;
	t_node				**node_to_give;

	node = NULL;
	if ((node = create_node(SEMI)) == NULL)
		return (ERROR);
	node_to_give = (node->left == NULL ? &(node->left) : &(node->right));
	if (check_c_pipe(l_expr, node_to_give))
	{
		if ((*l_expr)->type == SEMI && ft_strlen((*l_expr)->data) != 1)
		{
			printf("error in check expr\n");
			parse_error((*l_expr)->data);
			clear_node(&node);
			return (ERROR);
		}
		if ((*l_expr)->type == SEMI)
		{
			node->data = ft_strdup_ignchar((*l_expr)->data, '\\');
			*tree = node;
			if (move_in_list(l_expr))
				if (check_expr(l_expr, &(node->right)) == ERROR)
					return (ERROR);
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
	return (ERROR);
}

t_node				*parser(t_e_list **l_expr)
{
	if (DEBUG_PARSER == 1)
		printf("------- PARSER ------\n");

	t_node				*node;
	t_e_list			*head_l_expr;

	if (*l_expr == NULL)
		return (NULL);
	head_l_expr = *l_expr;
	node = NULL;
	if ((check_expr(l_expr, &node)) == FALSE)
		clear_tree(&node);

	// DEBUG !!!!!!!!!
	if (DEBUG_PARSER == 1)
	{
		t_e_list *tmp = head_l_expr;
		while(tmp){printf("[%s -> %d] --> ", (tmp)->data, (tmp)->type); tmp = (tmp)->next;}
		printf("\n");
	}
	// fin DEBUG !!!!!!!!!

	return (node);
}
