#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include "shell.h"
#include "libft.h"



//int					pipe_function(int fd_in, int fd_out, t_node *tree,
//					t_lst_fd **lstfd)
int					pipe_function(int pipefd_tab[2][2], t_node *tree,
		t_lst_fd **lstfd)
{
	if (DEBUG_PIPE == 1)
		ft_putendl_fd("------- PIPE FUNCTION -------", 2);

	(void)tree;
	(void)lstfd;
	int					pfd[2];

	if (pipefd_tab && pipefd_tab[1] && pipefd_tab[1][0] > 0)
	{
		pipefd_tab[0][0] = pipefd_tab[1][0];
		pipefd_tab[0][1] = pipefd_tab[1][1];
dprintf(2, "case 0 : (%d)(%d)\n", pipefd_tab[0][0], pipefd_tab[0][1]);
	}

	if (pipe(pfd) == ERROR)
	{
		ft_putendl_fd("pipe error", 2);
		return (ERROR);
	}
	pipefd_tab[1][0] = pfd[0];
	pipefd_tab[1][1] = pfd[1];
dprintf(2, "case 1 :(%d)(%d)\n", pipefd_tab[1][0], pipefd_tab[1][1]);
	return (TRUE);
}


/*
 *
 *
//		ft_putendl_fd("TOTO1", 2);
ft_putendl_fd("fd_in", 2);
ft_putnbr_fd(fd_in, 2);
ft_putendl_fd("", 2);
ft_putendl_fd("fd_out", 2);
ft_putnbr_fd(fd_out, 2);
ft_putendl_fd("", 2);

ft_putendl_fd("pfd1", 2);
ft_putnbr_fd(pfd[1], 2);
ft_putendl_fd("", 2);
ft_putendl_fd("pfd0", 2);
ft_putnbr_fd(pfd[0], 2);
ft_putendl_fd("", 2);
ft_putendl_fd("fd_in", 2);
ft_putnbr_fd(fd_in, 2);
ft_putendl_fd("", 2);
ft_putendl_fd("fd_out", 2);
ft_putnbr_fd(fd_out, 2);
ft_putendl_fd("", 2);
 *

 ft_putendl_fd("", 2);
 ft_putstr_fd("pfd[0]",2);
 ft_putnbr_fd(pfd[0], 2);
 ft_putendl_fd("", 2);
 ft_putstr_fd("pfd[1]",2);
 ft_putnbr_fd(pfd[1], 2);

 if (pipefd_tab[0])
 {
 ft_putendl_fd("", 2);
 ft_putstr_fd("pipefd_tab[0][0]",2);
 ft_putnbr_fd(pipefd_tab[0][0], 2);
 ft_putendl_fd("", 2);
 ft_putstr_fd("pipefd_tab[0][1]",2);
 ft_putnbr_fd(pipefd_tab[0][1], 2);
 }
 if (pipefd_tab[1])
 {
 ft_putendl_fd("", 2);
 ft_putstr_fd("pipefd_tab[1][0]",2);
 ft_putnbr_fd(pipefd_tab[1][0], 2);
 ft_putendl_fd("", 2);
 ft_putstr_fd("pipefd_tab[1][1]",2);
 ft_putnbr_fd(pipefd_tab[1][1], 2);
 ft_putendl_fd("", 2);
 }
 */
