#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4

static int			right_red_fd_pushbk(t_lst_fd **lstfd, t_lst_fd **tmpfd, int flags,
					char *filename, int insert)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- RIGHT RED FD PUSHBCK -------", 2);

	int 				fd;

	if (access(filename, F_OK) != ERROR && access(filename, W_OK) == ERROR)
	{ft_putendl("NON!!!!!!!!\n");
		return (ERROR);
	}
	if ((fd = open(filename, flags,
	S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == ERROR)
		return (ERROR);
	insert_in_lstfd(lstfd, tmpfd, fd, filename, insert);
	return (TRUE);
}

int					right_red_fd(t_lst_fd **lstfd, t_lst_fd **tmpfd, t_node *tree,
					t_node *red_arg, int insert)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- RIGHT RED FD -------", 2);

	int 				flags;
	int					ret_fn;
	char				*filename;

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
		insert_in_lstfd(lstfd, tmpfd, ft_atoi((filename) + 1), filename, insert);

	else if (ret_fn == CLOSE)
		insert_in_lstfd(lstfd, tmpfd, -1, red_arg->data, insert);
	else
		return (right_red_fd_pushbk(lstfd,tmpfd, flags, filename, insert));
	return (TRUE);
}
