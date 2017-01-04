#include "shell.h"
#include "libft.h"
#include <fcntl.h>

int					reset_std_fd(void)
{
	if (DEBUG_RED == 1)
		printf("------- RESET STD FD -------\n");

	int					fd;
	int					std_fd;

	fd = -1;
	std_fd = STDIN_FILENO;
	while (std_fd <= STDERR_FILENO)
	{
		if ((fd = open(savior_tty(NULL, FALSE), O_RDWR)) == -1)
			return (FALSE);
		if (dup2(fd, std_fd) == -1)
			return (FALSE);
		if (fd > STDERR_FILENO)
			close(fd);
		std_fd++;
	}
	return (TRUE);
}

int				tree_traversal(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_TREE == 1)
		printf("------- TREE TRAVERSAL -------(%d)\n", tree->type);
	t_lst_fd	*saved_lstfd;

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
	if (tree->type == CMD)
	{
		if ((manage_red_file(lstfd, tree)) == ERROR)
			return (ERROR);
		saved_lstfd = *lstfd;

		t_lst_fd *tmp = *lstfd;while(tmp){printf("[filename->%s]--[fd->%d]\n", tmp->filename, tmp->fd);tmp=tmp->next;}

		if ((manage_cmd(tree, lstfd)) == ERROR)
			return (ERROR);
		close_lstfd(&saved_lstfd);
		reset_std_fd();
	}

	return (TRUE);
}
