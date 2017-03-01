#include "shell.h"
#include "libft.h"

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
			return (sh_error(FALSE, 6, NULL, NULL));
		tmpfd = lstfd;
	}
	else
	{
		ret = lstfd_pushfront(lstfd, fd, filename);
		return (ret);
	}
	return (TRUE);
}
