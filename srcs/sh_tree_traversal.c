#include <fcntl.h>
#include "shell.h"
#include "libft.h"

// créer 3 fct tree_travers_semi tree_travers_pipe tree_travers_cmd
int					tree_traversal(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_TREE == 1)
		printf("------- TREE TRAVERSAL -------\n");

	t_lst_fd			*saved_lstfd;

	saved_lstfd = NULL;
	savior_tty(ttyname(0), TRUE);
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
		if ((manage_red_file(lstfd, tree)) == ERROR)
			return (ERROR);
		saved_lstfd = *lstfd;

		//		t_lst_fd *tmp = *lstfd;while(tmp){printf("[filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);tmp=tmp->next;}

		if ((pipe_function(tree, STDIN_FILENO, lstfd)) == ERROR)
			return (ERROR);
	}
	if (tree->type == CMD || (tree->type >= RRED && tree->type <= DLRED))
	{
		if ((manage_red_file(lstfd, tree)) == ERROR)
			return (ERROR);
		saved_lstfd = *lstfd;

		t_lst_fd *tmp = *lstfd;while(tmp){printf("[filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);tmp=tmp->next;}

		if (tree->type == CMD)
			if ((manage_cmd(tree, lstfd)) == ERROR)
				return (ERROR);
		*lstfd = saved_lstfd;
		close_lstfd(lstfd);
		reset_std_fd();
	}

	return (TRUE);
}
