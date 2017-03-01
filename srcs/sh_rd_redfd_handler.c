#include <fcntl.h>
#include "shell.h"
#include "libft.h"
#define	FD 3
#define CLOSE 4

int					check_file_name(char **filename, char *str)
{
	if (DEBUG_RED == 1)
		ft_putendl_fd("------- CHECK FILE NAME -------", 2);

	if ((*filename = ft_strsub(str, 1, ft_strlen(str))) == NULL)
		return (sh_error(FALSE, 6, NULL, NULL));
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
	if (savior_tty(NULL, FALSE) == NULL)
		return (FALSE);
	while (std_fd <= STDERR_FILENO)
	{
		if ((fd = open(savior_tty(NULL, FALSE), O_RDWR)) == ERROR)
			return (sh_error(FALSE, 3, NULL, NULL));
		if (dup2(fd, std_fd) == ERROR)
			return (sh_error(FALSE, 7, NULL, NULL));
		if (fd > STDERR_FILENO)
			close(fd);
		std_fd++;
	}
	return (TRUE);
}
