#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4

int					del_lstfd(t_lst_fd **lstfd)
{
//	if (DEBUG_RED == 1)
		ft_putendl_fd("------------ DEL LSTFD -------", 2);

	t_lst_fd			*tmpfd;

	tmpfd = NULL;
	while (lstfd && *lstfd)
	{
		tmpfd = *lstfd;
		*lstfd = (*lstfd)->next;
		ft_strdel(&(tmpfd->filename));
		free(tmpfd);
		tmpfd = NULL;
	}
	lstfd = NULL;
	dprintf (2, "in del lstfd ((((%p-%p)))\n", lstfd, tmpfd);
	return (TRUE);
}

t_lst_fd			*lstfd_new(int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD NEW -------", 2);

	t_lst_fd			*new_fd;

	if ((new_fd = (t_lst_fd*)malloc(sizeof(t_lst_fd))) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (NULL);
	new_fd->fd = fd;
	if ((new_fd->filename = ft_strdup(filename)) == NULL)
		/* RET: error EXIT: true MSG: "malloc fail" */
		return (NULL);
	new_fd->next = NULL;
	return (new_fd);
}

int					close_lstfd(t_lst_fd **lstfd)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd ("----- CLOSE LSTFD -----", 2);

	t_lst_fd			*tmp;

	tmp = NULL;
	if (lstfd)
	{
		while (lstfd && *lstfd)
		{
			if ((*lstfd)->fd > 2)
				close((*lstfd)->fd);
			ft_strdel(&((*lstfd)->filename));
			tmp = *lstfd;
			*lstfd = (*lstfd)->next;
			free(tmp);
			tmp = NULL;
		}
		*lstfd = NULL;
	}
	return (TRUE);
}

int					lstfd_pushbck(t_lst_fd **lstfd, int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD PUSHBCK -------", 2);

	t_lst_fd			*new_fd;
	t_lst_fd			*tmp;

	new_fd = NULL;
	tmp = *lstfd;
	if (!(lstfd && *lstfd))
		*lstfd = lstfd_new(fd, filename);
	else
	{
		while (tmp && tmp->next)
			tmp = tmp->next;
		new_fd = lstfd_new(fd, filename);
		tmp->next = new_fd;
	}
	return (TRUE);
}

int					lstfd_pushfront(t_lst_fd **lstfd, int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD PUSHFRONT -------", 2);

	t_lst_fd			*new_fd;

	new_fd = NULL;
	if (lstfd && !(*lstfd))
		*lstfd = lstfd_new(fd, filename);
	else
	{
		new_fd = lstfd_new(fd, filename);
		new_fd->next = *lstfd;
		*lstfd = new_fd;
	}
	return (TRUE);
}

t_lst_fd			*lstfd_insert(t_lst_fd **lstfd, t_lst_fd **tmpfd, int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD INSERT -------", 2);

	t_lst_fd			*new_fd;

	new_fd = NULL;
	if (lstfd && !(*lstfd))
	{
		*lstfd = lstfd_new(fd, filename);
		return (*lstfd);
	}
	else if (lstfd && *lstfd && tmpfd && *tmpfd)
	{
		new_fd = lstfd_new(fd, filename);
		new_fd->next = (*tmpfd)->next;
		(*tmpfd)->next = new_fd;
	}
	return (new_fd);
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
		if ((fd = open(savior_tty(NULL, FALSE), O_RDWR)) == ERROR)
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
