#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

//ONLY FOR ANTIBUG
int					tree_traversal_verif(t_node *tree)
{
	printf("------- TREE TRAVERSAL VERIF ------\n");

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

t_node				*read_n_check(int *nb_hrd, char *read_buff)
{
	if (DEBUG == 1)
		printf("------- READ N CHECK ------\n");

	t_e_list			*l_expr;
	t_node				*tree;

	l_expr = NULL;
	if (tokenizer(read_buff, &l_expr) == ERROR)
		return (NULL);
	if (lexer(&l_expr) == ERROR)
		return (NULL);
	tree = parser(nb_hrd, &l_expr);
	if (DEBUG_TREE_VERIF == 1)
		tree_traversal_verif(tree);
	return (tree);
}

int					check_after_read(t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- CHECK AFTER READ ------\n");

	t_node				*tree;
	t_node				*node;
	t_lst_fd			*lstfd;

	lstfd = NULL;
	if ((tree = read_n_check(&(stline->hrd.nb), stline->line)) == NULL)
		return (ERROR);
	node = tree;
	heredoc_handler(stline, &node, history);
	tree_traversal(tree, &lstfd);
	return (TRUE);
}

int					fct_read(int hrd, t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- FCT READ ------\n");

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
	if (key == RETURN && (stline->line)[0] == 0)
		return (FALSE);
	if (ret <= 0) // il faut pas un < strict?
		return (ERROR);
	if (hrd == TRUE)
		return (TRUE);
	if (check_after_read(stline, history) == ERROR)
		return (ERROR);
	return (TRUE);
}
