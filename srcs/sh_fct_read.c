#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				tree_traversal_verif(t_node *tree)
{
	if (DEBUG2 == 1)
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
	//	ft_putstr("[");
	//	ft_putstr(tree->data);
	//	ft_putstr("]");
	//	ft_putstr(" --> ");
	return (0);
}

t_node          *read_n_check(char *read_buff)
{
	if (DEBUG == 1)
		printf("------- READ N CHECK ------\n");
	t_e_list		*l_expr;
	t_node			*tree;

	l_expr = NULL;
	lexer_1(read_buff, &l_expr);
	//	t_e_list *tmp = l_expr;
	lexer_2(&l_expr);
	//	tmp = l_expr;
	printf("avant parser\n");
	tree = parser(&l_expr);
	printf("apres parser\n");
	if (DEBUG2 == 1)
	{
		tree_traversal_verif(tree);
		printf("\napres tree traversal verif\n");
	}
	//	tbl = lst_to_tbl(arg);
	//	free_lst(&arg);
	//	return (tbl);
	return (tree);
}

int				check_home(char **cmd)
{
	if (DEBUG == 1)
		printf("------- CHECK HOME ------\n");
	int			i;

	i = 0;
	while (cmd[i])
	{
		if (cmd[i][0] == '~')
		{
			ft_putstr("21sh: ");
			ft_putstr(cmd[0]);
			ft_putendl(": no $HOME variable set");
			return (-1);
		}
		i++;
	}
	return (0);
}

int				check_after_read(t_line *stline)
{
	if (DEBUG == 1)
		printf("------- CHECK AFTER READ ------\n");
	t_node          *tree;

	if ((tree = read_n_check(stline->line)) == NULL)
		return (-1);
	tree_traversal(tree);
	return (0);
}

int				fct_read(t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- FCT READ ------\n");
	int				key;
	int				ret;
	t_duo			*env;

	env = savior(NULL, FALSE);
	ret = 0;
	stline->curs_x = 3;
	(void)history;
	while ((ret = read(0, &key, 8)) > 0)
	{
		if (key == CTRL_D && stline->line[0] == '\0')
			bi_exit(NULL, &env);
		else if (key == CTRL_D)
			key = DEL;
		if (event(key, stline, history) == 1)
			break ;
		key = 0;
	}
	if (ret <= 0)
		bi_exit(NULL, &env);
	if (check_after_read(stline) == -1)
		return (-1);
	return (0);
}
