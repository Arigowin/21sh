#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int					parser_ret_fct(int ret, t_node **tree, t_node **node,
					t_node **to_free)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- PARSER RET FCT ------\n", 2);

	if (tree && *tree && node && *node)
		*tree = *node;
	clear_node(to_free);
	return (ret);
}

int					move_in_list(t_e_list **l_expr)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- MOVE IN LIST ------\n", 2);

	if ((*l_expr) && (*l_expr)->next)
	{
		(*l_expr) = (*l_expr)->next;
		return (TRUE);
	}
	return (FALSE);
}


//longeur ok si clear_node (l183) go in retun et si erreur final go in return
static int			check_command(int *nb_hrd, t_e_list **l_expr, t_node **tree) //static ac check c pipe
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK COMMAND -----", 2);

	t_node				*save;
	t_node				*node;
	int					ret;

	save = *tree;
	node = create_node(CMD);
	if ((ret = check_red(nb_hrd, l_expr, &(node->left))) != TRUE)
		*tree = save;
	if ((*l_expr)->type == CMD)
	{
		if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
		{
			clear_node(&node); // verif_si_ok
			return (sh_error(6, NULL, NULL));
		}
		if ((ret = check_next(nb_hrd, l_expr, &node, &(node->right))) < 0)
//			return (parser_ret_fct(ret, NULL, NULL, &node));
		{
			clear_node(&node); // ce clear ne cause pas de pb
			return (ret);
		}
//		return (parser_ret_fct(TRUE, tree, &node, NULL));
		*tree = node;
		return (TRUE);
	}
	if (ret == TRUE && (*l_expr)->type != CMD)
		//return (parser_ret_fct(TRUE, tree, &(node->left), &node));
	{
		*tree = node->left;
		clear_node(&node); // ce clear ne cause pas de pb
		return (TRUE);
	}
	clear_node(&node); // verif_si_ok
	if (ret != NO_PRINT)
		//return (sh_error(26, (*l_expr)->data, "1er appel"));
		return (sh_error(26, (*l_expr)->data, NULL));
	return (ret);
}

//longeur ok si erreur (l220) go in retun et si erreur final go in return
static int			check_c_pipe(int *nb_hrd, t_e_list **l_expr, t_node **tree)  // static ac check expr
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK C PIPE ------", 2);

	t_node				*node;
	t_node				**node_to_give;
	int					ret;

	node = create_node(PIPE);
	node_to_give = (node->left == NULL ? &(node->left) : &(node->right));
	if ((ret = check_command(nb_hrd, l_expr, node_to_give)) == TRUE)
	{
		if ((*l_expr)->type == PIPE)
		{
			if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
				return (sh_error(6, NULL, NULL));
			*tree = node;
			if (!(move_in_list(l_expr) && (ret = check_c_pipe(nb_hrd, l_expr, &(node->right)))))
			{
				clear_node(&node);
				return (sh_error(26, (*l_expr)->data, NULL));
			}
			return (ret);
		}
		//return (parser_ret_fct(ret, tree, node_to_give, &node));
		*tree = *node_to_give;
		clear_node(&node);
		return (ret);
	}
	clear_node(&node);
	if (ret != NO_PRINT)
		return (sh_error(26, (*l_expr)->data, NULL));
	return (ret);
}

static int			check_logic(int *nb_hrd, t_e_list **l_expr, t_node **tree)  // static ac check expr
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK LOGIC ------", 2);

	t_node				*node;
	t_node				**node_to_give;
	int					ret;

	node = create_node(SEMI);
	node_to_give = (node->left == NULL ? &(node->left) : &(node->right));
	if ((ret = check_c_pipe(nb_hrd, l_expr, node_to_give)) == TRUE)
	{
		if ((*l_expr)->type == LOGIC_OR || (*l_expr)->type == LOGIC_AND)
		{
			if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
				return (sh_error(6, NULL, NULL));
			*tree = node;
			if (!(move_in_list(l_expr) && (ret = check_logic(nb_hrd, l_expr, &(node->right)))))
			{
				clear_node(&node);
				return (sh_error(26, (*l_expr)->data, NULL));
			}
			return (ret);
		}
		//return (parser_ret_fct(ret, tree, node_to_give, &node));
		*tree = *node_to_give;
		clear_node(&node);
		return (ret);
	}
	clear_node(&node);
	if (ret != NO_PRINT)
		return (sh_error(26, (*l_expr)->data, NULL));
	return (ret);
}

//longeur ok si erreur (l258) go in retun et si erreur final go in return
static int			check_expr(int *nb_hrd, t_e_list **l_expr, t_node **tree) // static ac parser
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- CHECK EXPR ------", 2);

	t_node				*node;
	t_node				**node_to_give;
	int					ret;

	ret = 0;
	node = create_node(SEMI);
	node_to_give = (node->left == NULL ? &(node->left) : &(node->right));
	if ((*l_expr)->type == SEMI || ((*l_expr)->type != SEMI
	&& (ret = check_logic(nb_hrd, l_expr, node_to_give)) == TRUE)) // == TRUE))))
	{
		if ((*l_expr)->type == SEMI)
		{
			if ((node->data = ft_strdup((*l_expr)->data)) == NULL)
				return (sh_error(6, NULL, NULL));
			*tree = node;
			if ((ret = move_in_list(l_expr))
			&& ((ret = check_expr(nb_hrd, l_expr, &(node->right)) < 0)))
				return (ret);
			return (TRUE); // si on return ret ici, on ne peut plus faire 'ls;ls'
		}
		*tree = *node_to_give;
		clear_node(&node);
		return (ret);
	}
	clear_node(&node);
	if (ret != NO_PRINT)
		return (sh_error(26, (*l_expr)->data, NULL));
	return (ret);
}

int					parser(int *nb_hrd, t_e_list **l_expr, t_node **tree)
{
	if (DEBUG_PARSER == 1)
		ft_putendl_fd("------- PARSER ------", 2);

	int					ret;

	if (*l_expr == NULL)
		return (FALSE);
	ret = check_expr(nb_hrd, l_expr, tree);
//	if (ret != TRUE)
//		return (ret);

	// ANTIBUG !!!!!!!!!
	if (DEBUG_PARSER == 1)
	{
		t_e_list			*head_l_expr;
	head_l_expr = *l_expr;
		t_e_list *tmp = head_l_expr;
		while(tmp){printf("parser[%s -> %d] --> ", (tmp)->data, (tmp)->type); tmp = (tmp)->next;}
		printf("\n");
	}
	// fin ANTIBUG !!!!!!!!!

	return (ret);
}
