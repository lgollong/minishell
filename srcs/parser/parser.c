/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 12:05:55 by tilman            #+#    #+#             */
/*   Updated: 2024/12/12 23:30:01 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// get cmd_path from the cmd_array in the t_cmmnds-struct
void	get_commands_in_struct(void *content)
{
	t_cmmnds	*cmmnd_struct;

	cmmnd_struct = (t_cmmnds *)content;
	if (cmmnd_struct->cmd_array && !isbuiltin(cmmnd_struct)
		&& ft_strlen(cmmnd_struct->cmd_array[0]) != 0)
		cmmnd_struct->cmd_path = get_path(
				cmmnd_struct->cmd_array[0], cmmnd_struct->uni->envp);
}

// check the input for open quotes
void	check_open_quotes(t_uni *uni)
{
	if (cut_allow_checker('x', 0) == 1)
	{
		print_error_msg(NULL, "detected unclosed quotes");
		uni->stop = 1;
	}
}

// get the promt-message
char	*prompting(void)
{
	char	*dir;
	char	*temp;
	char	*str;
	char	*prompt;

	dir = getcwd(NULL, 0);
	temp = ft_strjoin("\x01\033[1;35m\x02", dir);
	str = ft_strjoin (temp, "$> \x01\033[0m\x02");
	prompt = ft_strjoin("\x01\033[1;35m\x02>Mini"
			"\x01\033[0m\x02\x01\033[1;36m\x02Shell@~ \x01\033[0m\x02", str);
	if (dir)
		free(dir);
	if (temp)
		free(temp);
	if (str)
		free(str);
	return (prompt);
}

// set the new value to the $_ variable
void	set_dollar_underscore_env(t_uni *uni)
{
	t_cmmnds	*cmd_strct;
	int			arr_len;

	cmd_strct = (t_cmmnds *)(uni->cmd_lst)->content;
	if (cmd_strct->cmd_array)
	{
		arr_len = array_length(cmd_strct->cmd_array);
		if (arr_len != 0)
			uni->envp = change_in_env("_=",
					ft_strdup(cmd_strct->cmd_array[arr_len - 1]), uni->envp, 0);
	}
}

void	print_list(t_list *lst)
{
	t_list	*tmp;
	t_cmmnds	*cmd;

	(void)cmd;
	tmp = lst;
	while (tmp)
	{
		// cmd = tmp->content;
		// printf("Command Array: ");
		// if (cmd->cmd_array)
		// {
		// 	for (int i = 0; cmd->cmd_array[i]; i++)
		// 	{
		// 		printf("%s ", cmd->cmd_array[i]);
		// 	}
		// }
		// printf("\n");
		// printf("Type: %d\n", cmd->type);
		// printf("Scope: %d\n", cmd->scope);
		tmp = tmp->next;
	}
}

// function for input-parsing
void	parser(t_uni *uni)
{
	char		*prompt;

	prompt = prompting();
	signal(SIGINT, &sig_handler_rl);
	signal(SIGQUIT, SIG_IGN);
	uni->input_string = readline(prompt);
	free(prompt);
	signal(SIGINT, &sig_handler_after_rl);
	if (!uni->input_string)
		exit_minishell (0, uni);
	if (ft_strlen(uni->input_string) <= 0)
		return ;
	add_history(uni->input_string);
	uni->commands = split_input(uni->input_string);
	handle_metachars(uni, uni->commands);
	check_open_quotes(uni);
	cmd_array_to_struct(uni);
	ft_lstiter(uni->cmd_lst, remove_quotes_from_structs);
	ft_lstiter(uni->cmd_lst, get_commands_in_struct);
	set_dollar_underscore_env(uni);
}
