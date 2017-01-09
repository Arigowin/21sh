#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"

//ONLY FOR DEBUG
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

t_node				*read_n_check(char *read_buff)
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
	tree = parser(&l_expr);
	if (DEBUG_TREE_VERIF == 1)
		tree_traversal_verif(tree);
	return (tree);
}

int					check_home(char **cmd)
{
	if (DEBUG == 1)
		printf("------- CHECK HOME ------\n");

	int					i;

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

int					check_after_read(t_line *stline)
{
	if (DEBUG == 1)
		printf("------- CHECK AFTER READ ------\n");

	t_node				*tree;
	t_lst_fd			*lstfd;

	lstfd = NULL;
	if ((tree = read_n_check(stline->line)) == NULL)
		return (ERROR);
	tree_traversal(tree, &lstfd);
	return (TRUE);
}

int					fct_read(t_line *stline, t_history **history)
{
	if (DEBUG == 1)
		printf("------- FCT READ ------\n");

	int					key;
	int					ret;
	int					event_ret;
//	char				*var1 = "ls";
//	char				*var2 = "-l";
//	char				*var3 = "/dev/fd/";
//	char				*(cmd[]) = {var1, var2, var3, NULL};

	ret = 0;
	key = 0;
//	father_n_son(cmd);
	while ((ret = read(STDIN_FILENO, &key, sizeof(int))) > 0)
	{
		//printf("key = %d\n", key);
		if ((event_ret = event(key, stline, history)) == BREAK)
			break ;
		else if (event_ret == CONTINUE)
			continue ;
		key = 0;
	}
//	printf("read ret : %d\n", ret);
	if (key == RETURN && (stline->line)[0] == 0)
		return (FALSE);
	if (ret <= 0) // il faut pas un < strict?
		return (ERROR);
	if (check_after_read(stline) == ERROR)
		return (ERROR);
	return (0);
}
