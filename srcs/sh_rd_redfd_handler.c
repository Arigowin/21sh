#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4

static t_lst_fd		*lstfd_new(int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD NEW -------", 2);

	t_lst_fd			*new;

	if ((new = (t_lst_fd*)malloc(sizeof(t_lst_fd))) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (NULL);
	new->fd = fd;
	if ((new->filename = ft_strdup(filename)) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (NULL);
	new->next = NULL;
	return (new);
}

int					close_lstfd(t_lst_fd **lstfd)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd ("----- CLOSE LSTFD -----", 2);

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

int					lstfd_pushbck(t_lst_fd **lstfd, int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD PUSHBCK -------", 2);

	t_lst_fd			*new;
	t_lst_fd			*tmp;

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

int					lstfd_pushfront(t_lst_fd **lstfd, int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD PUSHFRONT -------", 2);

	t_lst_fd			*new;

	new = NULL;
	if (lstfd && !(*lstfd))
		*lstfd = lstfd_new(fd, filename);
	else
	{
		new = lstfd_new(fd, filename);
		new->next = *lstfd;
		*lstfd = new;
	}
	return (TRUE);
}

t_lst_fd			*lstfd_insert(t_lst_fd **lstfd, t_lst_fd **tmpfd, int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD INSERT -------", 2);

	t_lst_fd			*new;

	new = NULL;
	if (lstfd && !(*lstfd))
	{
		*lstfd = lstfd_new(fd, filename);
		return (*lstfd);
	}
	else if (lstfd && *lstfd && tmpfd && *tmpfd)
	{
		new = lstfd_new(fd, filename);
		new->next = (*tmpfd)->next;
		(*tmpfd)->next = new;
	}
	return (new);
}

int					insert_in_lstfd(t_lst_fd **lstfd, t_lst_fd **tmpfd, int fd, char *filename, int insert)
{
	int 				ret;

	ret = -1;
	if (insert == TRUE)
	{
		if ((*tmpfd = lstfd_insert(lstfd, tmpfd, fd, filename)) == NULL)
			/* RET: error EXIT: true MSG: "malloc fail" */
			return (ERROR);
		tmpfd = lstfd;
	}
	else
	{
		ret = lstfd_pushfront(lstfd, fd, filename);
		return (ret);
	}
	return (TRUE);
}

int					check_file_name(char **filename, char *str)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- CHECK FILE NAME -------", 2);

	if ((*filename = ft_strsub(str, 1, ft_strlen(str))) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
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

int					reset_std_fd(void)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- RESET STD FD -------", 2);

	int					fd;
	int					std_fd;

	fd = -1;
	std_fd = STDIN_FILENO;
	while (std_fd <= STDERR_FILENO)
	{
		if ((fd = open(savior_tty(NULL, FALSE, TRUE), O_RDWR)) == ERROR)
			/* RET: error EXIT: false MSG: "open fail" */
			return (ERROR);
		if (dup2(fd, std_fd) == ERROR)
			/* RET: error EXIT: false MSG: "dup2 fail" */
			return (ERROR);
		if (fd > STDERR_FILENO)
			close(fd);
		std_fd++;
	}
	return (TRUE);
}
