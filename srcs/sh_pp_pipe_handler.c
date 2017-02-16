#include "shell.h"
#include "libft.h"

int 				pfd_handler(int pipefd_tab[2][2])
{
	if (DEBUG == 1)
		ft_putendl_fd("------- PFD HANDLER ------", 2);

	if (pipefd_tab && pipefd_tab[0][0] < 0 && pipefd_tab[1][0] >= 0)
	{
		close(pipefd_tab[1][0]);
		if (dup2(pipefd_tab[1][1], STDOUT_FILENO) == ERROR)
			return (sh_error(TRUE, 7, NULL, NULL));
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] >= 0)
	{
		close(pipefd_tab[0][1]);
		if(dup2(pipefd_tab[0][0], STDIN_FILENO) == ERROR)
			return (sh_error(TRUE, 7, NULL, NULL));
		close(pipefd_tab[1][0]);
		if (dup2(pipefd_tab[1][1], STDOUT_FILENO) == ERROR)
			return (sh_error(TRUE, 7, NULL, NULL));
	}
	if (pipefd_tab && pipefd_tab[0][0] >= 0 && pipefd_tab[1][0] < 0)
	{
		close(pipefd_tab[0][1]);
		if(dup2(pipefd_tab[0][0], STDIN_FILENO) == ERROR)
			return (sh_error(TRUE, 7, NULL, NULL));
	}
	return (TRUE);
}

int 				pfd_close(int pipefd_tab[2][2])
{
	if (pipefd_tab && pipefd_tab[0][0] >= 0)
	{
		close(pipefd_tab[0][0]);
		close(pipefd_tab[0][1]);
	}
	return (TRUE);
}
