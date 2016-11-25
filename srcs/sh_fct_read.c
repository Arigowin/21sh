#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

int				tree_traversal_verif(t_node *tree)
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

t_node          *read_n_check(char *read_buff)
{
	if (DEBUG == 1)
		printf("------- READ N CHECK ------\n");
	t_e_list		*l_expr;
	t_node			*tree;

	l_expr = NULL;
	if (lexer_1(read_buff, &l_expr) == ERROR)
		return (NULL);
	if (lexer_2(&l_expr) == ERROR)
		return (NULL);
	tree = parser(&l_expr);
	if (DEBUG2 == 1)
		tree_traversal_verif(tree);
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
	int				event_ret;

	ret = 0;
	while ((ret = read(0, &key, sizeof(int))) > 0)
	{
		if ((event_ret = event(key, stline, history)) == BREAK)
			break ;
		else if (event_ret == CONTINUE)
			continue ;
		key = 0;
	}
	if (key == RETURN && (stline->line)[0] == 0)
		return (FALSE);
	if (ret <= 0)
		return (ERROR);
	if (check_after_read(stline) == ERROR)
		return (ERROR);
	return (0);
}
