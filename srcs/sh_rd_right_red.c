#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4

static int			right_red_fd_pushbk(t_lst_fd **lstfd, int flags,
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
		lstfd_pushbck(lstfd, ft_atoi((filename) + 1), filename);
	else if (ret_fn == CLOSE)
		lstfd_pushbck(lstfd, -1, red_arg->data);
	else
		return (right_red_fd_pushbk(lstfd, flags, filename));
	return (TRUE);
}
