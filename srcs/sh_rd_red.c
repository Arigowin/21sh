#include <fcntl.h>
#include "shell.h"
#include "libft.h"

int					fd_exist(int fd)
{
	if (DEBUG_RED == 1)
		printf("------------ FD EXIST ------------\n");

	if (isatty(fd) == 0)
	{
		ft_putstr("21sh: ");
		ft_putstr(ft_itoa(fd));
		ft_putendl(": Bad file descriptor");
		return (FALSE);
	}
	return (TRUE);
}

int					left_right_red(t_node *tree, t_lst_fd **lstfd, int stdfd)
{
	if (DEBUG_RED == 1)
		printf("------- RIGHT RED -------\n");

	int					fd;

	fd = stdfd;
	if (tree->type == RED_FD && ft_strcmp(tree->data, "&") != 0)
		fd = ft_atoi(tree->data);
	else if (tree->type == RED_FD && ft_strcmp(tree->data, "&") == 0)
	{
		// if STDIN_FILENO
		// 		bash: file: ambiguous redirect
		if (stdfd == STDIN_FILENO || dup2((*lstfd)->fd, STDERR_FILENO) == ERROR)
			return (ERROR);
	}
	if (tree->right && tree->type == RED_FD)
		tree = tree->right;
	if ((*lstfd)->fd == -1)
	{
		close(fd);
		return (TRUE);
	}
	if (dup2((*lstfd)->fd, fd) == ERROR)
		return (ERROR);
	if ((*lstfd)->fd > STDERR_FILENO && (stdfd == STDOUT_FILENO
	|| (((*lstfd)->filename)[0] != '&' && (*lstfd)->fd != -1)))
		close((*lstfd)->fd);
	return (TRUE);
}

int					redirect(t_node *tree, t_lst_fd **lstfd)
{
	if (DEBUG_RED == 1)
		printf("------- RED -------\n");

	int					fd;

	fd = (tree->type == LRED ? STDIN_FILENO : STDOUT_FILENO);
	if (tree && tree->right && (tree->type != DLRED))
	{
		if (left_right_red(tree->right, lstfd, fd) == ERROR)
			return (ERROR);
	}
	else if (tree && tree->right && tree->type == DLRED)
		;

	if (tree && tree->left && (*lstfd)->next)
	{
		*lstfd = (*lstfd)->next;
		if (redirect(tree->left, lstfd) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}
