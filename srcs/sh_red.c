#include "shell.h"
#include "libft.h"
#include "fcntl.h"

int		rred(char *filename, int red_fd, int flags)
{
	if (DEBUG_RED == 1)
		printf("------- RRED -------\n");
	int		fd;

	if (red_fd == ERROR)
		red_fd = 1;

	if ((fd = open(filename, flags, 0644)) == ERROR)
		return (ERROR);

	if (dup2(fd, red_fd) == ERROR)
		return (ERROR);

	return (red_fd);
}

int		lred(char *filename, int red_fd)
{
	if (DEBUG_RED == 1)
		printf("------- LRED -------\n");
	int		fd;

	if (red_fd == ERROR)
		red_fd = 0;

	if ((access(filename, F_OK)) == ERROR)
	{
		printf("21sh: no such file or directory: %s\n", filename);
		return (ERROR);
	}

	if ((fd = open(filename, O_RDONLY)) == ERROR)
		return (ERROR);

	if (dup2(fd, red_fd) == ERROR)
		return (ERROR);

	return (red_fd);
}

int		red(t_node *tree, t_intlst **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED -------\n");
	char	*filename;
	int		red_fd;
	int		fd_ret;

	filename = NULL;
	red_fd = ERROR;
	if (tree->right->type == RED_FD)
	{
		red_fd = ft_atoi(tree->right->data);
		filename = ft_strdup(tree->right->right->data);
	}
	else
	{
		filename = ft_strdup(tree->right->data);
	}

	if (tree->type == RRED)
	{
		fd_ret = rred(filename, red_fd, O_RDWR | O_CREAT);
		if (fd_ret != ERROR)
			ft_intlst_add(lstfd, fd_ret);
		else
			return (ERROR);
	}
	else if (tree->type == LRED)
	{
		fd_ret = lred(filename, red_fd);
		if (fd_ret != ERROR)
			ft_intlst_add(lstfd, fd_ret);
		else
			return (ERROR);
	}
	else if (tree->type == DRRED)
	{
		fd_ret = rred(filename, red_fd, O_RDWR | O_CREAT | O_APPEND);
		if (fd_ret != ERROR)
			ft_intlst_add(lstfd, fd_ret);
		else
			return (ERROR);
	}
	else if (tree->type == DLRED)
	{
	}

	if (tree->left)
	{
		red(tree->left, lstfd);
		return (TRUE);
	}
	else
		return(FALSE) ;
}
