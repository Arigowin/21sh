#include <fcntl.h>
#include "shell.h"
#include "libft.h"

// créer 3 fct tree_travers_semi tree_travers_pipe tree_travers_cmd
int					tree_traversal(t_node *tree, t_lst_fd **lstfd, int pipefd_tab[2][2])
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- TREE TRAVERSAL -------", 2);

	t_lst_fd			*saved_lstfd;

	saved_lstfd = NULL;
	savior_tty(ttyname(0), TRUE, TRUE);
	savior_tty(ttyname(1), TRUE, FALSE);

	if (tree->type == SEMI)
	{
		if (tree->left)
			if ((tree_traversal(tree->left, lstfd, pipefd_tab)) == ERROR)
				return (ERROR);
		if (tree->right)
			if ((tree_traversal(tree->right, lstfd, pipefd_tab)) == ERROR)
				return (ERROR);
	}

	if (tree->type == PIPE)
	{
		if ((manage_red_file(lstfd, tree)) == ERROR)
			return (ERROR);
		saved_lstfd = *lstfd;

//		t_lst_fd *tmp = *lstfd;while(tmp){printf("[filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);tmp=tmp->next;}

		if ((pipe_function(pipefd_tab, tree, lstfd)) == ERROR)
				return (ERROR);
		if ((tree_traversal(tree->right, lstfd, pipefd_tab)) == ERROR)
				return (ERROR);
	//	*lstfd = saved_lstfd;
//		close_lstfd(lstfd);
//		reset_std_fd();
	}

	if (tree->type == CMD || (tree->type >= RRED && tree->type <= DLRED))
	{
		if ((manage_red_file(lstfd, tree)) == ERROR)
			return (ERROR);
		saved_lstfd = *lstfd;

		t_lst_fd *tmp = *lstfd;while(tmp){printf("[filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);tmp=tmp->next;}

		if (tree->type == CMD)
			if ((manage_cmd(pipefd_tab, tree, lstfd)) == ERROR)
				return (ERROR);
		*lstfd = saved_lstfd;
		close_lstfd(lstfd);
		reset_std_fd();
	}
	return (TRUE);
}
