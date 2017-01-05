#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4
/*
*/
int					close_lstfd(t_lst_fd **lstfd)
{
	if (DEBUG_RED == 1)
		printf ("----- CLOSE LSTFD -----\n");
	t_lst_fd			*tmp;

	tmp = NULL;
	while (*lstfd)
	{
		if ((*lstfd)->fd > 2)
			close((*lstfd)->fd);
		free((*lstfd)->filename);
		tmp = *lstfd;
		*lstfd = (*lstfd)->next;
		free(tmp);
	}
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
	{
		str_addleft(*filename, '&');
		return (FD);
	}
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

	if (access(filename, F_OK) != ERROR && access(filename, W_OK) == ERROR)
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
		lstfd_pushbck(lstfd, ft_atoi((filename) + 1), filename);
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
		lstfd_pushbck(lstfd, ft_atoi((filename) + 1), filename);
	else if (ret_fn == CLOSE)
		lstfd_pushbck(lstfd, -1, red_arg->data);
	else
		return (left_red_fd_pushbk(lstfd, filename));
	return (TRUE);
}

static int		red_travesal(t_lst_fd **lstfd, t_node **tree)
{
	// printf ("data : (%s-%d)\n", (*tree)->data, (*tree)->type);
	if (*tree && (*tree)->left)
	{
		if ((manage_red_file(lstfd, (*tree)->left)) == ERROR)
			return (ERROR);
	}
	if (*tree && (*tree)->right)
	{
		if ((manage_red_file(lstfd, (*tree)->right)) == ERROR)
			return (ERROR);
	}
	return (TRUE);
}

int					manage_red_file(t_lst_fd **lstfd, t_node *tree)
{
	if (DEBUG_RED == 1)
		printf("------- MANAGE RED FILE -------\n");

	t_node		*red_arg;

	red_arg = NULL;
	//printf("data: (%s)\n", tree->data);
	if (!tree || (tree->type == RRED || tree->type == DRRED
				|| tree->type == LRED || tree->type == DLRED))
	{
		if (!tree->right)
			return (FALSE);
		red_arg = (tree->right->type == RED_ARG ? tree->right
				: tree->right->right);
		if (tree->type == RRED || tree->type == DRRED)
			right_red_fd(lstfd, tree, red_arg);
		if (tree->type == LRED)
			left_red_fd(lstfd, red_arg);
		if (tree->type == DLRED)
			;
	}
	if (red_travesal(lstfd, &tree) == ERROR)
		return (ERROR);
	return (TRUE);
}
