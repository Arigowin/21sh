CC =	/usr/bin/clang
RM =	/bin/rm
MAKE =	/usr/bin/make
MKDIR =	/bin/mkdir

NAME = 21sh

ROOT =		$(shell /bin/pwd)
OPATH =		$(ROOT)/objs
CPATH =		$(ROOT)/srcs
HPATH =		$(ROOT)/includes
LIBPATH =	$(ROOT)/libft
LIBHPATH =	$(LIBPATH)/includes

CFLAGS = -g -Wall -Werror -Wextra -I $(HPATH) -I $(LIBHPATH)
LIBS = -L $(LIBPATH) -lft -ltermcap

SRC = sh_main.c \
	  \
	  sh_error.c \
	  sh_saviors.c \
	  sh_init.c \
	  sh_signal.c \
	  sh_first_steps.c \
	  sh_file_history.c \
	  \
	  sh_bi_builtin.c \
	  sh_bi_options.c \
	  sh_bi_handle_env_modif.c \
	  sh_bi_cd.c \
	  sh_bi_env.c \
	  sh_bi_exit.c \
	  sh_bi_echo.c \
	  sh_bi_setenv.c \
	  sh_bi_unsetenv.c \
	  \
	  sh_lp_finite_state_automaton.c \
	  sh_lp_lexer.c \
	  sh_lp_parser.c \
	  sh_lp_parser_additional_items.c \
	  sh_lp_tokenizer.c \
	  sh_lp_tokenizer_spec.c \
	  \
	  sh_rd_heredoc.c \
	  sh_rd_red.c \
	  sh_rd_redfd_handler.c\
	  sh_rd_manage_lstfd.c\
	  \
	  sh_tc_add_del_in_cpy.c \
	  sh_tc_copy_paste.c \
	  sh_tc_delete_in_line.c \
	  sh_tc_highlight.c \
	  sh_tc_history.c \
	  sh_tc_move_in_history.c \
	  sh_tc_insert_in_line.c \
	  sh_tc_move_in_line.c \
	  sh_tc_move_up_down.c \
	  sh_tc_spec_key.c \
	  sh_tc_termcap.c \
	  \
	  sh_cmd_line_assemble.c \
	  sh_telist_handler.c \
	  sh_cmd.c \
	  sh_pipe.c \
	  \
	  sh_fct_read.c \
	  sh_father_n_son.c \
	  \
	  sh_event.c \
	  sh_tree_functions.c \
	  sh_tree_traversal.c

OFILES = $(patsubst %.c, $(OPATH)/%.o, $(SRC))

.PHONY: all clean fclean re lib.fclean

all: $(OPATH) $(NAME)

$(NAME): $(OFILES)
	@$(MAKE) -C $(LIBPATH) -j 8
	@echo "Creating OBJ files"
	@echo "Building $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "\033[36mAll is done!\033[0m"

$(OPATH)/%.o: $(CPATH)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(OPATH):
	@echo "Creating OBJ directory"
	@$(MKDIR) $@

clean:
	@echo "Deleting OBJ files"
	@$(RM) -rf $(OPATH)

lib.fclean:
	@$(MAKE) fclean -C $(LIBPATH)

fclean: clean lib.fclean
	@echo "Deleting $(NAME)"
	@$(RM) -f $(NAME)
	@echo "\033[36mAll clear!\033[0m"

re: fclean all

norm: all
	norminette **/*.[ch]

# lil memo
# $@ = rule's name
# $^ = all the rule dependecies
# $< = only the first dependence
# -j 8 => pour que la lib complie en multi thread ;)


# Color for c
#   reset	"\033[0m"
#   RED  	"\033[31m"
#   GRN  	"\033[32m"
#   YEL  	"\033[33m"
#   BLU  	"\033[34m"
#   MAG  	"\033[35m"
#   CYN  	"\033[36m"
#   WHT  	"\033[37m"
