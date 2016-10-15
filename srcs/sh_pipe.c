#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "shell.h"
#include "libft.h"
#include <sys/wait.h>

//    pipeline(argv, 1, STDIN_FILENO);
//void pipeline(char *ar[], int pos, int in_fd)
//{
//	int fd[2];
//	pid_t childpid = 0;
//
//	if (ar[pos+1] == NULL)
//	{ /*last command */
//		if (in_fd != STDIN_FILENO)
//		{
//			if (dup2(in_fd, STDIN_FILENO) != -1)
//				close(in_fd); /*successfully redirected*/
//			else
//				error_exit("dup2");
//		}
//		execlp(ar[pos], ar[pos], NULL);
//		error_exit("execlp last");
//	}
//	else
//	{
//		if ((pipe(fd) == -1) || ((childpid = fork()) == -1))
//		{
//			error_exit("Failed to setup pipeline");
//		}
//		if (childpid == 0)
//		{ /* child executes current command */
//			close(fd[0]);
//			if (dup2(in_fd, STDIN_FILENO) == -1) /*read from in_fd */
//				perror("Failed to redirect stdin");
//			if (dup2(fd[1], STDOUT_FILENO) == -1)   /*write to fd[1]*/
//			{
//				perror("Failed to redirect stdout");
//			}
//			else
//			{
//				execlp(ar[pos], ar[pos], NULL);
//				error_exit("Failed to execlp");
//			}
//		}
//		close(fd[1]);   /* parent executes the rest of commands */
//		close(in_fd);
//		pipeline(ar, pos+1, fd[0]);
//	}
//}

int			pipe_function(t_node *tree, int in_fd)
{
	if (DEBUG_PIPE == 1)
		printf("------- PIPE FUNCTION -------(%s)-(%d)\n", tree->data, in_fd);
	int		pfd[2];
	int		pid;
	int		stat_lock;

	if (tree->right == NULL || tree->right->type == CMD_ARG)
	{
		printf("PIPE ERROR : if\n");
		if (in_fd != STDIN_FILENO)
		{
			printf("PIPE ERROR : if if\n");
			if (dup2(in_fd, STDIN_FILENO) != -1)
				close(in_fd);
			else
			{
				printf("PIPE ERROR : dup2(in_fd, STDIN_FILENO)\n");
				return (FALSE);
			}
		}
		father_n_son(format_cmd(tree));
		return (FALSE);
	}
	else
	{
		printf("PIPE ERROR : else\n");
		if (pipe(pfd) == -1 || (pid = fork()) == -1)
		{
			printf("PIPE ERROR : pipe(pfd)\n");
			return (FALSE);
		}
		if (pid > 0)
			wait(&stat_lock);
		if (pid == 0)
		{
			close(pfd[0]);
			if (dup2(in_fd, STDIN_FILENO) == -1)
			{
				printf("PIPE ERROR : in else dup2(in_fd, STDIN_FILENO)\n");
				return (FALSE);
			}
			if (dup2(pfd[1], STDOUT_FILENO) == -1)
			{
				printf("PIPE ERROR : in else dup2(pdf[1], STDOUT_FILENO)\n");
				return (FALSE);
			}
			else
			{
				father_n_son_for_pipe(format_cmd(tree->left));
				return (FALSE);
			}
		}
		close(pfd[1]);
		close(in_fd);
		pipe_function(tree->right, pfd[0]);
	}
	return (TRUE);
}
