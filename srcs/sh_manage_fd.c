#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4

int					init_std_fd(int (*std_fd)[])
{
	if (DEBUG_RED == 1)
		printf("------- INIT STD FD -------\n");

	(*std_fd)[0] = dup(STDIN_FILENO);
	(*std_fd)[1] = dup(STDOUT_FILENO);
	(*std_fd)[2] = dup(STDERR_FILENO);
	return (TRUE);
}

int					reset_std_fd(int std_fd[])
{
	if (DEBUG_RED == 1)
		printf("------- RESET STD FD -------\n");

	dup2(std_fd[0], STDIN_FILENO);
	dup2(std_fd[1], STDOUT_FILENO);
	dup2(std_fd[2], STDERR_FILENO);
	close(std_fd[0]);
	close(std_fd[1]);
	close(std_fd[2]);
	return (TRUE);
}

static t_lst_fd		*lstfd_new(int fd, char *filename)
{
	if (DEBUG_RED == 1)
		printf("------- LSTFD NEW -------\n");

	t_lst_fd	*new;

	if ((new = (t_lst_fd*)malloc(sizeof(t_lst_fd))) == NULL)
		return (NULL);
	new->fd = fd;
	if ((new->filename = ft_strdup(filename)) == NULL)
		return (NULL);
	new->next = NULL;
	return (new);
}

int					lstfd_pushbck(t_lst_fd **lstfd, int fd, char *filename)
{
	if (DEBUG_RED == 1)
		printf("------- LSTFD PUSHBCK -------\n");

	t_lst_fd *new;
	t_lst_fd *tmp;

	new = NULL;
	tmp = *lstfd;
	if (!(lstfd && *lstfd))
		*lstfd = lstfd_new(fd, filename);
	else
	{
		while (tmp && tmp->next)
			tmp = tmp->next;
		new = lstfd_new(fd, filename);
		tmp->next = new;
	}
	return (TRUE);
}

int					check_file_name(char **filename, char *str)
{
	if (DEBUG_RED == 1)
		printf("------- CHECK FILE NAME -------\n");

	if ((*filename = ft_strsub(str, 1, ft_strlen(str))) == NULL)
		return (ERROR);
	if (ft_isstrnum(*filename))
		return (FD);
	if (ft_strcmp(*filename, "-") == 0)
		return (CLOSE);
	return (TRUE);
}

int					right_red_fd_pushbk(t_lst_fd **lstfd, int flags,
					char *filename)
{
	if (DEBUG_RED == 1)
		printf("------- RIGHT RED FD PUSHBCK -------\n");

	int 				fd;

	if ((access(filename, W_OK)) == ERROR)
	{printf("NON!!!!!!!!\n");
		return (ERROR);
	}
	if ((fd = open(filename, flags,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == ERROR)
		return (ERROR);
	lstfd_pushbck(lstfd, fd, filename);
	return (TRUE);
}

int					right_red_fd(t_lst_fd **lstfd, t_node *tree,
					t_node *red_arg)
{
	if (DEBUG_RED == 1)
		printf("------- RIGHT RED FD -------\n");

	int 		flags;
	int			ret_fn;
	char		*filename;

	filename = NULL;
	ret_fn = FALSE;
	if (tree->type == RRED)
		flags = O_WRONLY | O_TRUNC | O_CREAT;
	else
		flags = O_WRONLY | O_APPEND | O_CREAT;
	if ((red_arg->data) && (red_arg->data)[0] && (red_arg->data)[0] == '&')
		ret_fn = check_file_name(&filename, red_arg->data);
	else if ((filename = ft_strdup(red_arg->data)) == NULL)
		return (ERROR);
	if (ret_fn == ERROR)
		return (ERROR);
	else if (ret_fn == FD)
		lstfd_pushbck(lstfd, ft_atoi(filename), filename);
	else if (ret_fn == CLOSE)
		lstfd_pushbck(lstfd, -1, red_arg->data);
	else
		return (right_red_fd_pushbk(lstfd, flags, filename));
	return (TRUE);
}

int					left_red_fd_pushbk(t_lst_fd **lstfd, char *filename)
{
	if (DEBUG_RED == 1)
		printf("------- LEFT RED FD PUSHBCK -------\n");

	int 				fd;

	fd = 0;
	if ((access(filename, F_OK)) == ERROR)
	{
		ft_putstr_fd("21sh: no such file or directory: ", 2);
		ft_putendl_fd(filename, 2);
		return (ERROR);
	}
	if ((access(filename, R_OK)) == ERROR)
	{
		ft_putstr_fd(filename, 2);
		ft_putendl_fd("permission denied: ", 2);
		return (ERROR);
	}
	if ((fd = open(filename, O_RDONLY)) == ERROR)
		return (ERROR);
	lstfd_pushbck(lstfd, fd, filename);
	return (TRUE);
}

int					left_red_fd(t_lst_fd **lstfd, t_node *red_arg)
{
	if (DEBUG_RED == 1)
		printf("------- LEFT RED FD -------\n");

	int					ret_fn;
	char				*filename;

	ret_fn = FALSE;
	filename = NULL;
	if ((red_arg->data) && (red_arg->data)[0] && (red_arg->data)[0] == '&')
		ret_fn = check_file_name(&filename, red_arg->data);
	else if ((filename = ft_strdup(red_arg->data)) == NULL)
		return (ERROR);
	if (ret_fn == ERROR || ret_fn == TRUE)
		return (ERROR);
	else if (ret_fn == FD)
		lstfd_pushbck(lstfd, ft_atoi(filename), filename);
	else if (ret_fn == CLOSE)
		lstfd_pushbck(lstfd, -1, red_arg->data);
	else
		return (left_red_fd_pushbk(lstfd, filename));
	return (TRUE);
}

int					manage_red_file(t_lst_fd **lstfd, t_node *tree)
{
	if (DEBUG_RED == 1)
		printf("------- MANAGE RED FILE -------\n");

	t_node		*red_arg;

	red_arg = NULL;
	if (tree && tree->left)
	{
		if ((manage_red_file(lstfd, tree->left)) == ERROR)
			return (ERROR);
	}
	if (tree && tree->right)
	{
		if ((manage_red_file(lstfd, tree->right)) == ERROR)
			return (ERROR);
	}
	if (!tree || !tree->right)
		return (FALSE);
	red_arg = (tree->right->type == RED_ARG ? tree->right : tree->right->right);
	if (tree->type == RRED || tree->type == DRRED)
		return (right_red_fd(lstfd, tree, red_arg));
	 if (tree->type == LRED)
		return (left_red_fd(lstfd, red_arg));
	if (tree->type == DLRED)
		;
	return (TRUE);
}
