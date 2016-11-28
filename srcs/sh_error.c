#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include "shell.h"
#include "libft.h"

//out = 0 => on continue - out = 1 => on quitte
int					sh_error(int ret_code, char *msg, int out)
{
	t_duo				**env;

	env = savior(NULL, FALSE);
	if (ret_code == TRUE)
		return (TRUE);
	else if (ret_code == FALSE)
	{
		if (msg != NULL)
			ft_putendl(msg);
		if (out == 1)
			bi_exit(NULL, env);
		return(FALSE);
	}
	else if (ret_code == ERROR)
	{
		if (msg != NULL)
			ft_putendl(msg);
		if (out == 1)
			bi_exit(NULL, env);
		return (ERROR);
	}
	return (TRUE);
}
