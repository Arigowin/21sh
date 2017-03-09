#include <fcntl.h>
#include <stdio.h>
#include "shell.h"
#include "libft.h"

int					fd_exist(int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------------ FD EXIST ------------", 2);

	char				*tmp;
	char				*str_fd;

	tmp = ft_strdup_ignchar(filename +  1, '\\');
	str_fd = ft_itoa(fd);
	if (ft_strcmp(str_fd, tmp))
	{
		ft_strdel(&tmp);
		ft_strdel(&str_fd);
		return (ERROR);
	}
	ft_strdel(&tmp);
	if (isatty(fd) == 0)
	{
		sh_error(ERROR, 29, str_fd, NULL);
		ft_strdel(&str_fd);
		return (ERROR);
	}
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
	{
		if (tree->right->right)
			str = ft_strdup(tree->right->right->data);
	}
	else
	{
		if (tree->right)
			str = ft_strdup(tree->right->data);
	}
	if (tree && tree->type == RED_FD
			&& ft_strcmp(tree->data, "&"))
		fd = ft_atoi(tree->data);
	if (pipe(hrd_fd) == ERROR)
		return (error_clear_str(ERROR, 4, NULL, &str));
	if (str)
		ft_putstr_fd(str, hrd_fd[1]);
	ft_strdel(&str);
	if (dup2(hrd_fd[0], fd) == ERROR)
		return (sh_error(FALSE, 7, NULL, NULL));
	close(hrd_fd[0]);
	close(hrd_fd[1]);
	return (TRUE);
}

int					heredoc_handler(t_line *stline, t_node **tree,
		t_history **history)
{
	if (DEBUG_HEREDOC == 1)
		ft_putendl_fd("------------ HEREDOC HANDLER ----------", 2);

	static int		bln = 0;

	if (stline->hrd.nb <= 0)
		return (FALSE);
	if ((*tree)->type == DLRED)
		manage_hrd_document(bln, stline, tree, history);
	if ((tree_trav_hrd(stline, tree, history)) == ERROR)
		return (ERROR);
	return (TRUE);
}

int					redirect(t_node *tree, t_lst_fd *lstfd)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- REDIRECT -------", 2);

	int					fd;
	int					ret;

	if ((lstfd == NULL && tree->type != DLRED) || tree == NULL)
		return (FALSE);
	fd = ((tree->type == RRED || tree->type == DRRED) ? STDOUT_FILENO : STDIN_FILENO); // remplacer STDOUT et STDIN par leur équivalent int
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
	if (tree && tree->left) // && lstfd->fd != -1)
	{
		if ((ret = tree->type == DLRED) && redirect(tree->left, lstfd) == ERROR)
			return (ERROR);
		else if (ret != TRUE && lstfd && ((lstfd->next && tree->type != DLRED)
					|| tree->left->type == DLRED) && redirect(tree->left, lstfd->next) == ERROR) // remplacer ERROR par -1 et DLRED par l'int correspondant
			return (ERROR);
	}
	return (TRUE);
}
