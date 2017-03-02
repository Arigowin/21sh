#include "shell.h"
#include "libft.h"

int					str_dbltbl_ret(int ret, char **str, char ***tbl1,
					char ***tbl2)
{
	ft_strdel(str);
	free_tab(tbl1);
	free_tab(tbl2);
	return (ret);
}

int					telist_ret(int ret, t_e_list **lst1, char **str1,
					char **str2)
{
	expr_del(lst1);
	ft_strdel(str1);
	ft_strdel(str2);
	return (ret);
}

int					lstfd_node_ret(int ret, t_node **node, t_lst_fd **lstfd,
					char **str2)
{
	del_tree(node);
	del_lstfd(lstfd);
	ft_strdel(str2);
	return (ret);
}

int					dblstr_duo_ret(int ret, char **str1, char **str2,
					t_duo **duo1)
{
	ft_strdel(str1);
	ft_strdel(str2);
	duo_del(duo1);
	return (ret);
}

int					dblstr_hist_ret(int ret, char **str1, char **str2,
					t_history **hist)
{
	ft_strdel(str1);
	ft_strdel(str2);
	del_history(hist);
	return (ret);
}

int					error_clear_node(int ret, int index, char *err,
	   				t_node **to_free)
{
	clear_node(to_free);
	return (sh_error(ret, index, err, NULL));
}

int					error_clear_str(int ret, int index, char *err,
	   				char **to_free)
{
	ft_strdel(to_free);
	return (sh_error(ret, index, err, NULL));
}
