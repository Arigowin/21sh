#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4

static t_lst_fd		*lstfd_new(int fd, char *filename)
{
	if (DEBUG_RED == 1)
		printf("------- LSTFD NEW -------\n");

	t_lst_fd			*new;

	if ((new = (t_lst_fd*)malloc(sizeof(t_lst_fd))) == NULL)
		return (NULL);
	new->fd = fd;
	if ((new->filename = ft_strdup(filename)) == NULL)
		return (NULL);
	new->next = NULL;
	return (new);
}

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

int					lstfd_pushbck(t_lst_fd **lstfd, int fd, char *filename)
{
	if (DEBUG_RED == 1)
		printf("------- LSTFD PUSHBCK -------\n");

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
