#include "shell.h"
#include "libft.h"
#include "fcntl.h"

int		rred(char *filename, int red_fd)
{
	if (DEBUG_RED == 1)
		printf("------- RRED -------\n");
	int		fd;

	fd = open(filename, O_RDWR | O_CREAT, 0644);
	if (red_fd != -1)
		dup2(fd, red_fd);
	else
		dup2(fd, 1);

	return (fd);
}

int		lred(char *filename, int red_fd)
{
	if (DEBUG_RED == 1)
		printf("------- LRED -------\n");
	int		fd;

	if ((access(filename, F_OK)) == -1)
	{
		printf("21sh: no such file or directory: %s\n", filename);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (red_fd != -1)
		dup2(fd, red_fd);
	else
		dup2(fd, 0);

	return (fd);
}

int		red(t_node *tree, t_intlst **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED -------\n");
	char	*filename;
	int		red_fd;

	filename = NULL;
	red_fd = -1;
	if (tree->right->type == RED_FD)
	{
		red_fd = ft_atoi(tree->right->data);
		filename = ft_strdup(tree->right->right->data);
	}
	else
		filename = ft_strdup(tree->right->data);

	if (tree->type == RRED)
		ft_intlst_add(lstfd, rred(filename, red_fd));
	else if (tree->type == LRED)
		ft_intlst_add(lstfd, lred(filename, red_fd));
	else if (tree->type == DRRED)
	{
	}
	else if (tree->type == DRRED)
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
