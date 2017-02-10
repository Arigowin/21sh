#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "shell.h"
#include "libft.h"

//ONLY FOR ANTIBUG
int					tree_traversal_verif(t_node *tree)
{
	if (DEBUG_TREE_VERIF == 1)
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
//fin ANTIBUG

static int			read_n_check(int *nb_hrd, char *read_buff, t_node **tree) // static ac fct read
{
	if (DEBUG == 1)
		ft_putendl_fd("------- READ N CHECK ------", 2);

	t_e_list			*l_expr;
	t_e_list			*save;
	static int			hrd = 0;
	int					ret;

	l_expr = NULL;
	save = NULL;
	read_buff = ft_strtrim(read_buff);
	if (tree == NULL || read_buff == NULL)
		return (FALSE);
	if ((ret = tokenizer(&hrd, read_buff, &l_expr)) != TRUE)
		return (ret);
	save = l_expr;
	if ((ret = lexer(&l_expr)) != TRUE)
		return (ret);
	if ((ret = parser(nb_hrd, &l_expr, tree)) != TRUE)
	{
		expr_del(&save);
		return (ret);
	}
	savior_tree(*tree, TRUE);
	expr_del(&save);
	if (DEBUG_TREE_VERIF == 1)
		tree_traversal_verif(*tree);
	return (TRUE);
}

int					check_after_read(t_line *stline, t_history **history) //fct a passer en static (ac fct read) si on vire le test
{
	if (DEBUG == 1)
		ft_putendl_fd("------- CHECK AFTER READ ------", 2);

	t_node				*tree;
	t_node				*node;
	t_lst_fd			*lstfd;
	int					pipefd_tab[2][2];
	int					ret;

	lstfd = NULL;
	tree = NULL;
	node = NULL;
	init_pipefd(pipefd_tab);
	if ((ret = read_n_check(&(stline->hrd.nb), stline->line, &tree)) != TRUE)
	{
		del_tree(&tree);
		return (ret);
	}
	node = tree;
	if ((ret = heredoc_handler(stline, &node, history)) == ERROR)
		return (FALSE);
	if ((ret = tree_traversal(tree, &lstfd, pipefd_tab)) == ERROR)
	{
		del_tree(&tree);
		return (ret);
		/* MSG ret: ERROR exit: TRUE msg: "whatever i don't have any ideas left"
		 * free: stline + globalfd + tree + node */
	}
//	del_tree(&tree);
	del_lstfd(&lstfd);
	return (ret);
}

int					ctrl_c_hrd(t_line *stline)
{
	int					fd;

	fd = 0;
	if (stline->ctrl_c == TRUE)
	{
		close(0);
		if ((fd = open(ttyname(1), O_RDWR)) < 0)
			return (ERROR); // open failed
		if (dup2(fd, STDIN_FILENO) < 0)
			return (ERROR); // dup2 failed
		if (fd > STDERR_FILENO)
			close(fd);
		stline->ctrl_c = FALSE;
	}
	return (TRUE);
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
	ctrl_c_hrd(stline);
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
