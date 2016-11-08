#ifndef SHELL_H
# define SHELL_H

#define DEBUG 0
#define DEBUG2 0
#define DEBUG_BUILTIN 0
#define DEBUG_LEXER_PARSER 0
#define DEBUG_TREE_CREATION 0
#define DEBUG_TREE 0
#define DEBUG_TERMCAPS 0
#define DEBUG_HISTORY 0
#define DEBUG_PIPE 0
#define DEBUG_RED 0
#define DEBUG_CMD 0
#define DEBUG_COPY_PASTE 0
#include <stdio.h>

# define TRUE 1
# define FALSE 0
# define ERROR -1

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
#define CUTE 30747 // ALT + x

// - MAC OPTION + [LEFT|RIGHT]
// # define OP_LEFT 345
// # define OP_RIGHT 344
// - LINUX CTRL + [a|e]
# define OP_LEFT 1
# define OP_RIGHT 5

# define DEL 2117294875
# define HOME 2117163803
# define END 2117360411

# define PRT_LEN 3

# include "libft.h"
#include <sys/ioctl.h>

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
	SEMI
} 						types;

typedef struct			s_e_list // -> l_expr
{
	char				*data;
	types				type;
	struct s_e_list		*next;
}						t_e_list;

typedef struct			s_line
{
	char				*line;
	int					pos_line;
	int					curs_x; //first line start at  = stline->pos_line + PRT_LEN;
								// other line start at = 0
								// quote line start at len of "dquote> " or "quote> "
	int					curs_y;
	struct winsize		win;
	int					quote;
	char				*copy;
	int					cpy_pos;
	int					cpy_start;
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

/*
 ** sh_main
 */
t_duo					*savior(t_duo *env, int code);
t_line					*savior_stline(t_line *stline, int code);

/*
 ** sh_first_steps
 */
int				display_prompt(void);
char			**cpy_env(char **env);
int				manage_tilde(char **arg);
int				fill_path(char ***env);

/*
 ** sh_signal
 */
int				check_signal(int loc);

/*
** sh_builtin
*/
int				add_env(char *name, char *value);
int				change_env(char *name, char *value);
char			*get_env(char *name);
int				handle_builtin(char **cmd);

/*
 ** sh_lexer1
 */
int				lexer_1(char *read_buff, t_e_list **l_expr);
t_e_list		*expr_new(char *content);

/*
 ** sh_lexer2
 */
int				lexer_2(t_e_list **l_expr);

/*
 ** sh_create_tree
 */
t_node			*create_node(types type);
//int				add_node(char *data, types type, t_node **node, int side); // 0 pour right - 1 pour left

/*
** sh_fct_read
*/
t_node          *read_n_check(char *read_buff);
int				check_home(char **cmd);
int				check_after_read(t_line *stline);
int				fct_read(t_line *line, t_history **history);

/*
** sh_father_n_son
*/
int				father_n_son(char **cmd);
int				father_n_son_for_pipe(char **cmd);

/*
 ** sh_env
 */
int				bi_env(char **arg, t_duo **env);

/*
 ** sh_exit
 */
int				bi_exit(char **arg, t_duo **env);

/*
 ** sh_setenv
 */
int				bi_setenv(char **arg, t_duo **env);

/*
 ** sh_unsetenv
 */
int				bi_unsetenv(char **arg, t_duo **env);

/*
 ** sh_cd
 */
int				bi_cd(char **arg, t_duo **env);
/*
 ** sh_termcap
 */
int				init_term();
int				reset_term();

/*
 ** sh_tputs
 */
int				my_outc(int c);

/*
 ** sh_event
 */
int				event(int key, t_line *stline, t_history **history);
int				reset_stline(t_line *stline);

/*
 ** sh_modif_line
 */
int				backspace(t_line *stline);
int				insert(t_line *stline, char c, int pos);

/*
 ** sh_move_in_line
 */
int				move(int key, t_line *stline);

/*
** sh_move_up_down
*/
int				move_up_down(int key, t_line *stline);

/*
 ** sh_spec_key
 */
int				spec_key(int key, t_line *stline);

/*
 ** sh_history
 */
void			add_history(t_history **history, char *line);
int				nav_history(int key, t_history **history, t_line *stline);

/*
 ** sh_parser
 */
t_node			*parser(t_e_list **l_expr);
int				check_next(t_e_list **l_expr, t_node **tree, t_node **right_node);

/*
 ** sh_tree_traversal
 */
int				tree_traversal(t_node *tree);

/*
 ** sh_red
 */
int     		red(t_node *tree, t_intlst **lstfd);

/*
 ** sh_cmd
 */
char			**format_cmd(t_node *tree);
int				manage_cmd(t_node *tree);

/*
 ** sh_pipe
 */
int				pipe_function(t_node *tree, int in_fd);

/*
** sh_copy_paste
*/
int				copy_paste(int key, t_line *stline);
int				add_in_copy(t_line *stline, int dir);
int				del_in_copy(t_line *stline, int dir);

#endif

/*
x == fd x >= 0
y == fd y >= 0 and y <= 2     /dev/fd (http://stackoverflow.com/questions/7082001/file-descriptors)

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

cmd &x> ... error si & pas de fd

cmd &>& file // parse error near
cmd &>& y    // parse error near
*/

/*
il faut gerer encore
OK	cmd >
	cmd <
OK	<> // pas besoin de la gerer car on a pas le builtin exec
OK	cmd >&-
	> file (sans commande)
*/

/*
&< error
<&x
*/


/*
ls /tmp/ abc 2>&1 | less
*/


