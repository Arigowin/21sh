#include "shell.h"
#include "libft.h"
#include <fcntl.h>

int				tree_traversal(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_TREE == 1)
		printf("------- TREE TRAVERSAL -------(%d)\n", tree->type);
	int			saved_std[3];

	if (tree->type == SEMI)
	{
		if (tree->left)
			if ((tree_traversal(tree->left, lstfd)) == ERROR)
				return (ERROR);
		if (tree->right)
			if ((tree_traversal(tree->right, lstfd)) == ERROR)
				return (ERROR);
	}
	if (tree->type == PIPE)
	{
		init_std_fd(&saved_std);
		if ((manage_red_file(lstfd, tree)) == ERROR)
			return (ERROR);
		if ((pipe_function(tree, STDIN_FILENO)) == ERROR)
			return (ERROR);
		reset_std_fd(saved_std);
	}
	if (tree->type == CMD)
	{
		if ((manage_red_file(lstfd, tree)) == ERROR)
			return (ERROR);
		t_lst_fd *tmp = *lstfd;while(tmp){printf("[filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);tmp=tmp->next;}
		if ((manage_cmd(tree)) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}
