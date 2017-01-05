#ifndef SHELL_H
# define SHELL_H

#define DEBUG 0
#define DEBUG2 0
#define DEBUG_BUILTIN 0
#define DEBUG_LEXER_PARSER 0
#define DEBUG_PARSER 0
#define DEBUG_TREE_CREATION 0
#define DEBUG_TREE 0
#define DEBUG_TERMCAPS 0
#define DEBUG_HISTORY 0
#define DEBUG_PIPE 0
#define DEBUG_RED 0
#define DEBUG_CMD 0
#define DEBUG_COPY_PASTE 0
#define DEBUG_KEY 0
#define DEBUG_HEREDOC 1
#include <stdio.h>

# define TRUE 1
# define FALSE 0
# define ERROR -1
# define BREAK 2
# define CONTINUE 3

# define IGN " \t\n" // j'ai enlevé le \0 des IGN
# define SEP "|&;>< \t\n\0"
# define SPECIAL "|&><;"
# define WAKA "><"
# define BUFF_SIZE 1024
# define MAX_PATH 1024

# define QUOTE 39
# define DQUOTE 34

# define LEFT 4479771
# define RIGHT 4414235
# define UP 4283163
# define DOWN 4348699
# define CTRL_UP 28955 // alt + q
# define CTRL_DOWN 30491 // alt + w

# define BACKSPACE 127
# define TAB 9
# define RETURN 10
# define CTRL_D 4

#define HIGHLIGHT 26651 // ALT + h
#define PASTE 30235 // ALT + v
#define COPY 25371 // ALT + c
#define CUT 30747 // ALT + x

// - MAC OPTION + [LEFT|RIGHT]
// # define OP_LEFT 345
// # define OP_RIGHT 344
// - LINUX CTRL + [a|e]
# define CTRL_LEFT 1
# define CTRL_RIGHT 5

# define DEL 2117294875
# define HOME 2117163803
# define END 2117360411

# define PRT_LEN 2

# define COLOR "\033[0;30;46m"
# define RESET_COLOR "\033[0m"

# include "libft.h"
# include <sys/ioctl.h>

typedef enum
{
	NONE,
	EXP,
	RED,
	RED_FD,
	RED_ARG,
	CMD,
	CMD_ARG,
	PIPE,
	LAND,
	LDOR,
	LDAND,
	RRED,
	LRED,
	DRRED,
	DLRED,
	SEMI,
	ECHAP
} 						types;

typedef enum
{
	STANDARD,
	IN_QUOTE,
	IN_DQUOTE
}						states;

typedef struct			s_e_list // -> l_expr
{
	char				*data;
	types				type;
	struct s_e_list		*next;
}						t_e_list;

typedef struct			s_copy
{
	char				*cpy;
	char				*bkup;
	int					pos;
	int					start;
}						t_copy;

typedef struct			s_heredoc
{
	int					nb;
	char				*ptr;
	struct s_list		*deli;
}						t_heredoc;

typedef struct			s_line
{
	char				*line;
	int					pos_line;
	int					curs_x; //first line start at  = stline->pos_line + PRT_LEN;
								// other line start at = 0
								// quote line start at len of "> "
	int					curs_y;
	struct winsize		win;
	struct s_copy		copy;
	struct s_heredoc	hrd;
	int					quote;
}						t_line;

typedef struct			s_history
{
	char				*line;
	struct s_history	*prev;
	struct s_history	*next;
}						t_history;

typedef struct			s_node // -> node ou tree
{
	char				*data;
	types				type;
	struct s_node		*left;
	struct s_node		*right;
}						t_node;

typedef struct			s_key_fct
{
	int					key;
	int					(*fct)(t_line *stline, t_history **history);
}						t_key_fct;

typedef struct			s_lst_fd
{
	int					fd;
	char				*filename;
	struct s_lst_fd		*next;
}						t_lst_fd;

/*
** sh_main
*/
t_duo					*savior(t_duo *env, int code);
t_line					*savior_stline(t_line *stline, int code);
char					*savior_tty(char *tty, int code);

/*
** sh_first_steps
*/
int						display_prompt(void);
char					**cpy_env(char **env);
int						fill_path(char ***env);

/*
** sh_signal
*/
int						check_signal(int loc);

/*
** sh_builtin
*/
int						add_env(char *name, char *value);
int						change_env(char *name, char *value);
char					*get_env(char *name);
int						is_builtin(char **cmd);
int						handle_builtin(char **cmd);

/*
** sh_lexer1
*/
int						lexer_1(char *read_buff, t_e_list **l_expr);
t_e_list				*expr_new(char *content);

/*
** sh_lexer2
*/
int						lexer_2(t_e_list **l_expr);

/*
** sh_create_tree
*/
t_node					*create_node(types type);

/*
** sh_fct_read
*/
t_node					*read_n_check(char *read_buff);
int						check_home(char **cmd);
int						check_after_read(t_line *stline);
int						fct_read(t_line *line, t_history **history);

/*
** sh_father_n_son
*/
int						father_n_son(char **cmd);
int						father_n_son_for_pipe(char **cmd);
int						handle_fork(t_node *tree, t_lst_fd **lstfd);

/*
** sh_env
*/
int						bi_env(char **arg, t_duo **env);

/*
** sh_exit
*/
int						bi_exit(char **arg, t_duo **env);

/*
** sh_setenv
*/
int						bi_setenv(char **arg, t_duo **env);

/*
** sh_unsetenv
*/
int						del_env(t_duo **env, char *name);
int						bi_unsetenv(char **arg, t_duo **env);

/*
** sh_cd
*/
int						bi_cd(char **arg, t_duo **env);
/*
** sh_termcap
*/
int						init_term();
int						reset_term();

/*
** sh_tputs
*/
int						my_outc(int c);

/*
** sh_event
*/
int						event(int key, t_line *stline, t_history **history);
int						reset_stline(t_line *stline);
int						fct_ctrl_d(t_line *stline, t_history **history);

/*
** sh_modif_line
*/
int						fct_backspace(t_line *stline, t_history **history);
int						fct_insert(t_line *stline, char c, char **str,
						int *pos);

/*
** sh_move_in_line
*/
int						fct_left(t_line *stline, t_history **history);
int						fct_right(t_line *stline, t_history **history);
int						fct_ctrl_left(t_line *stline, t_history **history);
int						fct_ctrl_right(t_line *stline, t_history **history);

/*
** sh_move_up_down
*/
int						fct_ctrl_down(t_line *stline, t_history **history);
int						fct_ctrl_up(t_line *stline, t_history **history);

/*
** sh_spec_key
*/
int						fct_end(t_line *stline, t_history **history);
int						fct_home(t_line *stline, t_history **history);
int						fct_del(t_line *stline, t_history **history);

/*
** sh_history
*/
void					add_history(t_history **history, char *line);
int						fct_down(t_line *stline, t_history **history);
int						fct_up(t_line *stline, t_history **history);

/*
** sh_copy_paste
*/
int						str_addleft(char *tbl, char c);
int						fct_cut(t_line *stline, t_history **history);
int						fct_paste(t_line *stline, t_history **history);
int						fct_copy(t_line *stline, t_history **history);
int						fct_highlight(t_line *stline, t_history **history);
int						add_in_copy(t_line *stline, int dir);
int						del_in_copy(t_line *stline, int dir);

/*
** sh_parser
*/
t_node					*parser(t_e_list **l_expr);
int						check_next(t_e_list **l_expr, t_node **tree, t_node **right_node);

/*
** sh_tree_traversal
*/
int						tree_traversal(t_node *tree, t_lst_fd **lstfd);

/*
** sh_red
*/
int						fd_exist(int fd);
int     				redirect(t_node *tree, t_lst_fd **lstfd);

/*
** sh_manage_fd
*/
int						close_lstfd(t_lst_fd **lstfd);
int						lstfd_pushbck(t_lst_fd **lstfd, int fd, char *filename);
int						manage_red_file(t_lst_fd **lstfd, t_node *tree);

/*
** sh_cmd
*/
char					**format_cmd(t_node *tree);
int						manage_cmd(t_node *tree, t_lst_fd **lstfd);

/*
** sh_pipe
*/
int						pipe_function(t_node *tree, int in_fd, t_lst_fd **lfd);

/*
** sh_heredoc
*/
int						fill_heredoc(t_line *stline);
int						return_heredoc(t_line *stline);



#endif

/*
 * single quote https://www.gnu.org/software/bash/manual/html_node/Single-Quotes.html
 * double quote https://www.gnu.org/software/bash/manual/html_node/Double-Quotes.html
 */

/*
x == fd x >= 0
y == fd y >= 0     /dev/fd (http://stackoverflow.com/questions/7082001/file-descriptors)

	if (access("/dev/fd/3", F_OK) == -1)
		return (-1);
	// si access return false sa veut dire que le fd n'existe pas

OK cmd > file   le 1 dans file
OK cmd >& y     le 1 et 2 dans x
OK cmd >& file  le 1 et 2 dans file
OK cmd x> file  le x dans file
OK cmd x>& file le x est ignorer redirige le 1 et 2 dans file
OK cmd x>& y    le x est ignorer redirige le 1 et 2 dans y
OK cmd &> file  le 1 et 2 dans file
*/

/*
x == file or fd
cmd > &x error bash: syntax error near unexpected token `&'
cmd >&x == cmd >& x

cmd & > ... error
cmd &> ... OK

cmd &>& file
cmd &>& y
*/

/*
il faut gerer encore
	cmd <
	> file (sans commande)
*/

/*
&< error
*/


/*
Probleme de redirection de l'erreur qui devrait s'afficher dans le less
ls /tmp/ abc 2>&1 | less
*/

