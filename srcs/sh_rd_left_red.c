#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4

static int			left_red_fd_pushbk(t_lst_fd **lstfd, t_lst_fd **tmpfd, char *filename, int insert)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LEFT RED FD PUSHBCK -------", 2);

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
	{
		//printf("fd dans left red fd pushbk :  (%d)\n", fd);
		insert_in_lstfd(lstfd, tmpfd, -1, filename, insert); // -1 a la place de filename?
		return (SYS_CALL_FAIL);
	}
	//if (*lstfd && *tmpfd)printf("BEFORE INSERT (%s)(%s)\n", (*lstfd)->filename,  (*tmpfd)->filename);
	insert_in_lstfd(lstfd, tmpfd, fd, filename, insert);
	//printf("AFTER INSERT (%s)(%s)\n", (*lstfd)->filename,  (*tmpfd)->filename);
	return (TRUE);
}

int					left_red_fd(t_lst_fd **lstfd, t_lst_fd **tmpfd, t_node *red_arg, int insert)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- LEFT RED FD -------", 2);

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
		insert_in_lstfd(lstfd, tmpfd, ft_atoi((filename) + 1), filename, insert);
	else if (ret_fn == CLOSE)
		insert_in_lstfd(lstfd, tmpfd, -1, red_arg->data, insert);
	else
		return (left_red_fd_pushbk(lstfd, tmpfd, filename, insert));
	return (TRUE);
}
