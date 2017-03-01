#include <fcntl.h>
#include <stdio.h>
#include "shell.h"
#include "libft.h"

int					fd_exist(int fd)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------------ FD EXIST ------------", 2);

	if (isatty(fd) == 0)
		return (sh_error(FALSE, 29, ft_itoa(fd), NULL));
	return (TRUE);
}

static int			left_right_red(t_node *tree, t_lst_fd *lstfd, int stdfd) // static ac redirect
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LEFT RIGHT RED -------", 2);

	int					fd;

	fd = stdfd;
	if (tree->type == RED_FD && ft_strcmp(tree->data, "&") != 0)
		fd = ft_atoi(tree->data);
	else if (tree->type == RED_FD && ft_strcmp(tree->data, "&") == 0)
	{
		if (stdfd == STDIN_FILENO)
			return (sh_error(FALSE, 7, NULL, NULL));
		if (dup2(lstfd->fd, STDERR_FILENO) == ERROR)
			return (sh_error(FALSE, 7, NULL, NULL));
	}
	if (tree->right && tree->type == RED_FD)
		tree = tree->right;
	if (lstfd->fd == -42)
	{
		if (fd >= 0)
			close(fd);
		return (TRUE);
	}
	if (lstfd->fd >= 0 && dup2(lstfd->fd, fd) == ERROR)
		return (sh_error(FALSE, 7, NULL, NULL));
	return (TRUE);
}

static int			heredoc_red(t_node *tree, int fd) // static ac redirect
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- HEREDOC RED -------", 2);

	char				*str;
	int					hrd_fd[2];

	str = NULL;
	if (tree->type == RED_FD)
		str = ft_strdup(tree->right->right->data);
	else
		str = ft_strdup(tree->right->data);
	if (tree && tree->type == RED_FD
			&& ft_strcmp(tree->data, "&"))
		fd = ft_atoi(tree->data);
	if (pipe(hrd_fd) == ERROR)
		/* RET: error EXIT: false MSG: "pipe fail"
		 * FREE: str */
		return (ERROR);
	if (str)
		write(hrd_fd[1], str, ft_strlen(str));
	ft_strdel(&str);
	if (dup2(hrd_fd[0], fd) == ERROR)
		return (sh_error(FALSE, 7, NULL, NULL));
	close(hrd_fd[0]);
	close(hrd_fd[1]);
	return (TRUE);
}

int					redirect(t_node *tree, t_lst_fd *lstfd)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- REDIRECT -------", 2);

	int					fd;

	if ((lstfd == NULL && tree->type != DLRED) || tree == NULL)
		return (FALSE);
	fd = ((tree->type == RRED || tree->type == DRRED) ? STDOUT_FILENO : STDIN_FILENO);
	if (tree && tree->right && (tree->type != DLRED))
	{
		if (left_right_red(tree->right, lstfd, fd) == ERROR)
			return (ERROR);
	}
	else if (tree && tree->right && tree->type == DLRED)
	{
		if (heredoc_red(tree->right, fd) == ERROR)
			return (ERROR);
	}
	if (tree && tree->left && lstfd) // && lstfd->fd != -1)
	{
		if (tree->type == DLRED && redirect(tree->left, lstfd) == ERROR)
			return (ERROR);
		if (((lstfd->next && tree->type != DLRED) || tree->left->type == DLRED)
				&& redirect(tree->left, lstfd->next) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}
