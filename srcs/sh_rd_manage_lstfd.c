#include "shell.h"
#include "libft.h"

t_lst_fd			*lstfd_new(int fd, char *filename)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LSTFD NEW -------", 2);

	t_lst_fd			*new_fd;

	if ((new_fd = (t_lst_fd*)malloc(sizeof(t_lst_fd))) == NULL)
	{
		sh_error(FALSE, 6, NULL, NULL);
		return (NULL);
	}
	new_fd->fd = fd;
	if ((new_fd->filename = ft_strdup(filename)) == NULL)
	{
		sh_error(FALSE, 6, NULL, NULL);
		return (NULL);
	}
	new_fd->next = NULL;
	return (new_fd);
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

int					del_lstfd(t_lst_fd **lstfd) // la fct close_lstfd est a privilégier elle close le fd avant de le free...
{
	if (DEBUG_RED == 1)
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
	return (TRUE);
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
