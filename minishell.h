/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 15:29:14 by lgollong          #+#    #+#             */
/*   Updated: 2024/12/12 23:29:49 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "./libft/libft.h"
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>
# include <sys/ioctl.h>

extern int	g_exitcode;

//tokens
# define CMD 0
# define INFILE_RE 1
# define HERE_DOC_RE 2
# define WR_TO_OUT 3
# define AP_TO_OUT 4
# define PIPE 5
# define AND 6
# define OR 7
# define PAR_OPEN 8
# define PAR_CLOSE 9

typedef struct s_uni
{
	char		*input_string;
	char		**commands;
	t_list		*cmd_lst;
	char		**envp;
	pid_t		pid;
	int			stop;
	int			scope_p;
	int			last_exit_status;
}	t_uni;

typedef struct s_cmmnds
{
	char				**cmd_array;
	char				*cmd_path;
	int					inf;
	int					outf;
	t_uni				*uni;
	int					broken;
	struct s_cmmnds		*left;
	struct s_cmmnds		*right;
	int					type;
	int					scope;
}	t_cmmnds;

// main.c
void		free_stuff(t_uni	*uni);
void		exit_minishell(int exit_code, t_uni *uni);
int			main(int argc, char *argv[], char **envp);

// initialisation.c
t_uni		*init_uni_struct(int argc, char *argv[], char **envp);
char		*get_own_env(t_uni *uni, char *name);

// signal_handler.c
void		sig_handler_after_rl(int num);
void		sig_handler_rl(int num);
void		sig_handler_here_doc(int num);
void		quit_handler(int num);

// helpers.c
void		clean_array(char ***array);
int			array_length(char	**array);
void		del_str_from_array(char **array, int del_pos, int del_quant);
char		**add_str_to_array(char **array, char *str);
char		*str_extend(char **str, char *extension);

void		print_error_msg(char *cmmnd, char *msg);
void		print_long_error_msg(char *cmmnd, char *arg, char *msg);

// parser/...
void		parser(t_uni *uni);
char		**split_input(char *str);
void		handle_metachars(t_uni *uni, char **array);
char		*check_for_metachars(t_uni *uni, char *str, int *j, int *cnt);
char		*replace_part_of_str(char *str, int start,
									int end, char *rep_prt);
void		remove_quotes_from_structs(void *content);
void		rem_quote_from_str(char *str, int *j);
int			cut_allow_checker(char mark, int reset);
void		cmd_array_to_struct(t_uni *uni);
t_cmmnds	*create_cmmnd_struct(t_uni *uni);
int			open_file_and_save_fd(t_uni *uni,
				t_cmmnds *cmmnd_struct, int i, int type);
void		start_here_doc(t_uni *uni, t_cmmnds *cmd_strct, int i);
char		*get_path(char *cmmnd, char *envp[]);
int			syntax_error(t_uni *uni, char *token);
void		cmmnd_to_struct(t_cmmnds *cmmnd_struct, char *command);

// executer/...
void		close_fds(void *content);
void		executer(t_uni *uni);

//builtins
int			isbuiltin(t_cmmnds *node);
int			exec_builtin(t_cmmnds *node, int builtin, int forked);
int			ft_cd(t_cmmnds *node);
int			ft_pwd(t_cmmnds *node);
int			ft_echo(t_cmmnds *node);
int			ft_cd(t_cmmnds *node);
int			ft_env(t_cmmnds *node);
int			ft_unset(t_cmmnds *node);
int			ft_export(t_cmmnds *node);
int			ft_exit(t_cmmnds *node);
char		**change_in_env(char *set, char *replace,
				char **envp, int free_set);

//wildcard
int		handle_wildcard(char *command, t_cmmnds *content);

//parentheses

#endif