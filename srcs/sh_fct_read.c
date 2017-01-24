#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

//ONLY FOR ANTIBUG
int					tree_traversal_verif(t_node *tree)
{
	ft_putendl_fd("------- TREE TRAVERSAL VERIF ------", 2);

	printf("tree : %s - %d\n", tree->data, tree->type);
	if (tree && tree->left != NULL)
	{
		printf("{{%s left %s}}\n", tree->data, tree->left->data);
		tree_traversal_verif(tree->left);
	}
	if (tree && tree->right != NULL)
	{
		printf("{{%s right %s}}\n", tree->data, tree->right->data);
		tree_traversal_verif(tree->right);
	}
	return (0);
}

int					read_n_check(int *nb_hrd, char *read_buff, t_node **tree)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- READ N CHECK ------", 2);

	t_e_list			*l_expr;
	int					ret;

	l_expr = NULL;
	if (tree == NULL || read_buff == NULL)
		return (FALSE);
	if ((ret = tokenizer(read_buff, &l_expr)) != TRUE)
	{
		expr_del(&l_expr);
		return (ret);
	}
	if ((ret = lexer(&l_expr)) != TRUE)
	{
		expr_del(&l_expr);
		return (ret);
	}
	if ((ret = parser(nb_hrd, &l_expr, tree)) != TRUE) // juste garder ret = .... et return ret
	{
		expr_del(&l_expr);
		return (ret);
	}
	//if (l_expr)
	expr_del(&l_expr);
	if (DEBUG_TREE_VERIF == 1)
		tree_traversal_verif(*tree);
	return (TRUE);
}

int					check_after_read(t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK AFTER READ ------", 2);

	t_node				*tree;
	t_node				*node;
	t_global_fd			*globalfd;
	int					pipefd_tab[2][2];
	int					ret;

	globalfd = NULL;
	pipefd_tab[0][0] = -2;
	pipefd_tab[0][1] = -2;
	pipefd_tab[1][0] = -2;
	pipefd_tab[1][1] = -2;
	if ((ret = read_n_check(&(stline->hrd.nb), stline->line, &tree)) != TRUE)
		return (ret);
	node = tree;
	heredoc_handler(stline, &node, history);
	if ((ret = tree_traversal(tree, &globalfd, pipefd_tab)) == ERROR)
	{
		return (ret);
		/* MSG ret: ERROR exit: TRUE msg: "whatever i don't have any ideas left"
		 * free: stline + globalfd + tree + node */
	}
	return (ret);
}

int					fct_read(int hrd, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		ft_putendl_fd("------- FCT READ ------", 2);

	int					key;
	int					ret;
	int					event_ret;

	ret = 0;
	key = 0;
	while ((ret = read(STDIN_FILENO, &key, sizeof(int))) > 0)
	{
		if ((event_ret = event(key, stline, history)) == BREAK)
			break ;
		else if (event_ret == CONTINUE)
			continue ;
		key = 0;
	}
	if (key == RETURN && (stline->line)[0] == '\0')
		return (FALSE);
	if (ret <= 0)
		return (ERROR);
	if (hrd == TRUE)
		return (TRUE);
	if (check_after_read(stline, history) == ERROR)
		return (ERROR);
	return (TRUE);
}
