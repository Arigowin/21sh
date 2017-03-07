#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define FD 3
#define CLOSE 4

int					check_file_name(char **filename, char *str)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- CHECK FILE NAME -------", 2);

	if ((*filename = ft_strsub(str, 1, ft_strlen(str))) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
	if (ft_isstrnum(*filename))
	{
		str_addleft(*filename, '&');
		return (FD);
	}
	if (ft_strcmp(*filename, "-") == 0)
		return (CLOSE);
	return (TRUE);
}

int					reset_std_fd(void)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- RESET STD FD -------", 2);

	int					fd;
	int					std_fd;

	fd = -1;
	std_fd = STDIN_FILENO;
	if (savior_tty(NULL, FALSE) == NULL)
		return (FALSE);
	while (std_fd <= STDERR_FILENO)
	{
		if ((fd = open(savior_tty(NULL, FALSE), O_RDWR)) == ERROR)
			return (sh_error(FALSE, 3, NULL, NULL));
		if (dup2(fd, std_fd) == ERROR)
			return (sh_error(FALSE, 7, NULL, NULL));
		if (fd > STDERR_FILENO)
			close(fd);
		std_fd++;
	}
	return (TRUE);
}

int					red_fd(int fd, t_node *tree, t_lst_fd **lstfd, t_types type)
{
	if (DEBUG_TREE == 1)
		ft_putendl_fd("------- RED FD -------", 2);

	int					ret;
	static int			fd_save = 0;
	static int			reset_save = TRUE;
	t_lst_fd			*pipe_fd;
	t_lst_fd			*tmp;

	pipe_fd = NULL;
	fd_save = ((fd == -21 || fd == -2) ? 0 : fd_save);
	reset_save = ((tree->type == CMD) ? TRUE : reset_save);
	if (tree && tree->type == PIPE)
	{
		tmp = *lstfd;
		while (pipe_fd && tmp && tmp != pipe_fd)
			tmp = tmp->next;
		pipe_fd = *lstfd;
		fd = -21;
	}
	if ((ret = fd_open(&fd, reset_save, tree)) == ERROR)
		return (push_in_lstfd(tree, lstfd, ret, &fd_save));
	if (tree && tree->right && tree->type == PIPE)
		if ((ret = red_fd(fd, tree->right, lstfd, type)) == ERROR)
			fd = -1;
	if (tree && tree->left && (ret = red_fd(fd, tree->left, lstfd, type)) == -1)
		fd = -1;
	return (push_in_lstfd(tree, lstfd, fd, &fd_save));
}
