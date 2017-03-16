#ifndef SHELL_H
# define SHELL_H

# define HISTORY_FILE_NAME "/.21sh_history"

# define CONTINUE 3
# define BREAK 2
# define TRUE 1
# define FALSE 0
# define ERROR -1
# define NO_RED_ARG -2
# define SYS_CALL_FAIL -3
# define NO_PRINT -4

# define IGN " \t\n"
# define SEP "|&;>< \t\n\0"
# define SPECIAL "|&><;"

# define SPECIAL2 "|><;"
# define LWAKA "><|&"
# define WAKA "><"
# define BUFF_SIZE 1024
# define MAX_PATH 1024

# define QUOTE 39
# define DQUOTE 34

# define LEFT 4479771
# define RIGHT 4414235
# define UP 4283163
# define DOWN 4348699
# define BACKSPACE 127
# define TAB 9
# define RETURN 10
# define CTRL_D 4
# define DEL 2117294875

# if defined(__unix__)

/*
** LINUX CTRL + [q|w]
*/
#  define CTRL_UP 28955
#  define CTRL_DOWN 30491

/*
** LINUX ALT + [h|v|c|x]
** pouette commentaire multilignes !
*/
#  define HIGHLIGHT 26651
#  define PASTE 30235
#  define COPY 25371
#  define CUT 30747

/*
** LINUX CTRL + [a|e]
*/
#  define CTRL_LEFT 1
#  define CTRL_RIGHT 5

/*
** LINUX
*/
#  define HOME 2117163803
#  define END 2117360411

# elif defined(__APPLE__)

/*
** MAC
*/
#  define CTRL_UP 16693
#  define CTRL_DOWN 16949

/*
** MAC OPT + [h|v|c|x]
*/
#  define HIGHLIGHT 39371
#  define PASTE 10127586
#  define COPY 42947
#  define CUT 8948194

/*
** MAC
*/
#  define CTRL_LEFT 17461
#  define CTRL_RIGHT 17205

/*
** MAC
*/
#  define HOME 4741915
#  define END 4610843
# endif

# define PRT_LEN 2

# define COLOR "\033[0;30;46m"
# define RESET_COLOR "\033[0m"

# include <sys/ioctl.h>
# include "libft.h"

typedef enum			e_types
{
	NONE,
	EXP,
	SEMI,
	PIPE,
	CMD,
	CMD_ARG,
	RED,
	RED_FD,
	RED_ARG,
	HRD_QUOTE,
	RRED,
	LRED,
	DRRED,
	DLRED,
	RWRED,
	DLRED_DOC,
	LOGIC_AND,
	LOGIC_OR,
	AMP
}						t_types;

typedef enum			e_states
{
	STANDARD,
	IN_QUOTE,
	IN_DQUOTE
}						t_states;

typedef struct			s_e_list
{
	char				*data;
	t_types				type;
	int					hrd_quote;
	struct s_e_list		*next;
}						t_e_list;

typedef struct			s_node
{
	int					quote;
	char				*data;
	t_types				type;
	struct s_node		*left;
	struct s_node		*right;
}						t_node;

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
	int					pos;
	int					ctrl_d;
	struct s_node		*deli;
	char				*line;
	char				*ptr;
}						t_heredoc;

typedef struct			s_line
{
	int					pos;
	int					curs_x;
	int					curs_y;
	int					mini_prt;
	int					quote;
	int					ctrl_c;
	char				*line;
	char				*curr_hist;
	struct winsize		win;
	struct s_copy		copy;
	struct s_heredoc	hrd;
}						t_line;

typedef struct			s_history
{
	char				*line;
	struct s_history	*prev;
	struct s_history	*next;
}						t_history;

typedef struct			s_key_fct
{
	int					key;
	int					(*fct)(char **str, int *pos, t_line *stline,
							t_history **history);
}						t_key_fct;

typedef struct			s_lst_fd
{
	int					fd;
	char				*filename;
	struct s_lst_fd		*next;
}						t_lst_fd;

/*
** sh_check_tty
*/
int						checktty(t_line *stline);

/*
** sh_end_pipe_quote
*/
int						check_end_pipe(char **str, int *pos);
int						check_nb_quote(char c, int back, int *quote, int fill);
int						quote_is_close(char **str);
int						handle_quote(int key, char **str, int *pos,
							t_line *stline);

/*
** sh_error
*/
int						sh_error(int ret, int index, char *err, char *bi);

/*
** free_return
*/
int						str_dbltbl_ret(int ret, char **str, char ***tbl1,
							char ***tbl2);
int						telist_ret(int ret, t_e_list **lst1, char **str1,
							char **str2);
int						lstfd_node_ret(int ret, t_node **node, t_lst_fd **lstfd,
							char **str2);
int						dblstr_duo_ret(int ret, char **str1, char **str2,
							t_duo **duo1);
int						dblstr_hist_ret(int ret, char **str1, char **str2,
							t_history **hist);

/*
** free_return_error
*/
int						error_clear_node(int ret, int index, char *err,
							t_node **to_free);
int						error_clear_str(int ret, int index, char *err,
							char **to_free);
int						error_clear_dblstr(int ret, int index, char **to_free,
							char **to_free_bis);
int						error_clear_tab(int ret, int index, char *err,
							char ***to_free);
void	 				exit_clear_stline(int ret, t_line **stline);

/*
** sh_file_history
*/
int						load_history(t_history **history);
int						save_history(void);

/*
** sh_savior
*/
t_duo					*savior_env(t_duo *env, int code);
t_line					*savior_stline(t_line *stline, int code);
char					*savior_tty(char *tty, int code);
t_node					*savior_tree(t_node *tree, int code);
t_history				**savior_history(t_history **history, int code);

/*
** sh_init
*/
int						init_env(char **env, t_duo **env_cpy);
int						init_stline(t_line *stline);
int						reset_stline(t_line *stline);
int						init_pipefd(int pipefd_tab[2][2]);

/*
** sh_first_steps
*/
int						miniprt_reset_stline(t_line *stline);
int						mini_prt_handler(char **str, int *pos, t_line *stline);
int						display_prompt(void);
char					**cpy_env(char **env);
int						fill_path(char ***env);

/*
** sh_signal
*/
int						check_signal(int loc);

/*
** sh_bi_builtin
*/
int						is_builtin(char **cmd);
int						handle_builtin(char **cmd);
int						check_builtin(int fd, char **cmd, int pipefd_tab[2][2],
							t_lst_fd **lstfd);

/*
** sh_bi_options
*/
int						check_opt(char **arg, int *i);

/*
** sh_bi_handle_env_modif
*/
int						change_env(char *name, char *value);
char					*get_env(char *name);

/*
** sh_bi_cd
*/
int						bi_cd(char **arg, t_duo **env);

/*
** sh_bi_echo
*/
int						bi_echo(char **arg, t_duo **env);

/*
** sh_bi_env
*/
int						bi_env(char **arg, t_duo **env);

/*
** sh_bi_exit
*/
int						del_stline(t_line **stline);
int						bi_exit(char **arg, t_duo **env);
int						exit_pgm(int exit_code);

/*
** sh_bi_setenv
*/
int						valid_env_name(char *str, char *bi);
int						bi_setenv(char **arg, t_duo **env);

/*
** sh_bi_unsetenv
*/
int						del_env(t_duo **env, char *name);
int						bi_unsetenv(char **arg, t_duo **env);

/*
** sh_t_e_list_handler
*/
int						expr_del(t_e_list **new);
t_e_list				*expr_new(char *content, int hrd);
int						expr_pushbk(t_e_list **l_expr, char *data_tmp, int hrd);

/*
** sh_lp_finite_state_automaton
*/
int						finite_state_automaton(int *hrd, char **read_buff,
							t_e_list **l_expr, char **data_tmp);
/*
** sh_lp_tokenizer
*/
int						concat(char **dest, char *s1, char *s2);
int						token_sep(int *hrd, char **read_buff, char **data_tmp,
							t_e_list **l_expr);
int						tokenizer(int *hrd, char *read_buff, t_e_list **l_expr);

/*
** sh_lp_tokenizer_spec
*/
int						token_backslash(t_states state, char **read_buff,
							char **data_tmp);
int						token_dollar(char **read_buff, char **data_tmp);
int						token_tilde(char **buff, char **data_tmp, int *bln);

/*
** sh_lp_lexer
*/
int						lexer(t_e_list **l_expr);

/*
** sh_lp_lexer_waka
*/
int						waka_lexer(t_e_list **l_expr);

/*
** sh_create_tree
*/
t_node					*create_node(t_types type);

/*
** sh_fct_read
*/
int						tree_traversal_verif(t_node *tree);
int						check_after_read(t_line *stline, t_history **history);
int						fct_read(int hrd, t_line *line, t_history **history);

/*
** sh_father_n_son
*/
int						handle_fork(int pipefd_tab[2][2], t_node *tree,
							t_lst_fd **lstfd, char **cmd);

/*
** sh_cmd_line_assemble
*/
int						null_input(int fd);
int						check_fct(int fd, char **cmd);

/*
** sh_tc_termcap
*/
int						my_outc(int c);
int						init_term(int full_init);
int						reset_term();

/*
** sh_event
*/
int						quote_is_close(char **str);
int						event(int key, t_line *stline, t_history **history);

/*
** sh_tc_insert_in_line
*/
int						fct_insert(char **s, int *pos, char c, t_line *stline);

/*
** sh_tc_delete_in_line
*/
int						fct_ctrl_d(char **s, int *pos, t_line *stline,
							t_history **history);
int						fct_backspace(char **s, int *pos, t_line *stline,
							t_history **history);
/*
** sh_tc_move_in_line
*/
int						fct_left(char **s, int *pos, t_line *l, t_history **h);
int						fct_right(char **s, int *pos, t_line *line,
							t_history **history);
int						fct_ctrl_left(char **s, int *pos, t_line *stline,
							t_history **history);
int						fct_ctrl_right(char **s, int *pos, t_line *stline,
							t_history **history);

/*
** sh_tc_move_up_down
*/
int						left_move_cdt(int pos, t_line *stline);
int						fct_down(char **s, int *pos, t_line *stline,
							t_history **hstory);
int						fct_up(char **s, int *p, t_line *l, t_history **h);

/*
** sh_tc_spec_key
*/
int						fct_end(char **s, int *pos, t_line *l, t_history **h);
int						fct_home(char **s, int *pos, t_line *l, t_history **h);
int						fct_del(char **s, int *pos, t_line *l, t_history **h);

/*
** sh_tc_history
*/
void					del_history(t_history **history);
void					add_history(t_history **h, char *line);
void					modif_history(t_history **hist, char *line, int mini);

/*
** sh_tc_move_in_history
*/
int						history_down(char **str, int *pos, t_line *stline,
							t_history **history);
int						history_up(char **s, int *p, t_line *l, t_history **h);

/*
** sh_tc_copy_paste
*/
int						fct_cut(char **s, int *pos, t_line *l, t_history **h);
int						fct_paste(char **s, int *pos, t_line *l, t_history **h);
int						fct_copy(char **s, int *pos, t_line *l, t_history **h);

/*
** sh_tc_add_del_in_copy
*/
int						str_addleft(char *tbl, char c);
int						str_delleft(char *tbl);
int						add_in_copy(char **s, int *p, t_line *stline, int dir);
int						del_in_copy(char **s, int *p, t_line *stline, int dir);

/*
** sh_tc_highlight
*/
int						hide_highlight(char **str, int *pos, t_line *stline,
							t_history **history);
int						fct_highlight(char **str, int *pos, t_line *stline,
							t_history **history);

/*
** sh_lp_parser
*/
int						parse_error(char *data);
int						parser(int *nb_hrd, t_e_list **l_expr, t_node **tree);

/*
** sh_lp_parser_additional_items
*/
int						check_red(int *nb_hrd, t_e_list **l_expr, t_node **t);
int						check_next(int *nb_hrd, t_e_list **l_expr, t_node **t,
							t_node **r_n);

/*
** sh_lp_parser_useful_fct
*/
int						fill_leaf(t_e_list **l_expr, t_node **node);
t_types					fill_red_type(char *data, int *nb_hrd);
int						parser_ret_fct(int ret, t_node **tree, t_node **node,
							t_node **to_free);
int						move_in_list(t_e_list **l_expr);

/*
** sh_lp_parser_additional_items
*/
int						parser_ret_fct(int ret, t_node **tree, t_node **node,
						t_node **to_free);

/*
** sh_free_tree
*/
int						clear_node(t_node **node);
int						del_tree(t_node **tree);

/*
** sh_tree_traversal
*/
int						tree_traversal(t_node *tree, t_lst_fd **lstfd,
							int pipefd[2][2]);

/*
** fd_open
*/
int						fd_open(int	*fd, int reset_save, t_node *tree);

/*
** sh_rd_red
*/
int						fd_exist(int fd, char *filename);
int						heredoc_handler(t_line *l, t_node **t, t_history **h);
int						redirect(t_node *tree, t_lst_fd *lstfd);

/*
** sh_rd_manage_lstfd
*/
int						del_lstfd(t_lst_fd **lstfd);
t_lst_fd				*lstfd_new(int fd, char *filename);
int						close_lstfd(t_lst_fd **lstfd);
int						lstfd_pushfront(t_lst_fd **lstfd, int fd, char *name);
int						push_in_lstfd(t_node *tree, t_lst_fd **lstfd, int fd,
							int *fd_save);

/*
** sh_rd_redfd_handler
*/
int						check_file_name(char **filename, char *str);
int						reset_std_fd(void);
int						red_fd(int fd, t_node *tree, t_lst_fd **lstfd,
							t_types type);
t_lst_fd				*lstfd_insert(t_lst_fd **lstfd, t_lst_fd **tmpfd,
							int fd, char *filename);

/*
** sh_rd_heredoc
*/
int						tree_trav_hrd(t_line *stline, t_node **tree,
							t_history **history);
int						manage_hrd_document(int bln, t_line *stline,
							t_node **tree, t_history **history);

/*
** sh_heredoc
*/
int						check_end_heredoc(t_line *stline);
int						fill_heredoc(t_line *stline);
int						return_heredoc(t_line *stline);

/*
** sh_cmd
*/
int						manage_cmd(int pipefd_tab[2][2], t_node *tree,
							t_lst_fd **lstfd);

/*
** sh_pipe
*/
int						pfd_handler(int pipefd_tab[2][2]);
int						pfd_close(int pipefd_tab[2][2]);
int						pipe_function(int pipefd_tab[2][2], t_node *tree,
							t_lst_fd **lstfd);

#endif
