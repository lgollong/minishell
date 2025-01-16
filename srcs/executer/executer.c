/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 14:16:32 by rwegat            #+#    #+#             */
/*   Updated: 2025/01/16 16:52:44 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
// iterates through the t_list and closes all open file-descriptors
void	close_fds(void *content)
{
	t_cmmnds	*cmd_strct;

	cmd_strct = (t_cmmnds *)content;
	if (cmd_strct->inf != -1 && cmd_strct->inf != 0)
		close(cmd_strct->inf);
	if (cmd_strct->outf != -1 && cmd_strct->outf != 1)
		close(cmd_strct->outf);
}

// checks if command is executable
int	is_not_executable(t_cmmnds *cmd_strct, int builtin)
{
	if (cmd_strct->broken != 0)
	{
		g_exitcode = cmd_strct->broken;
		return (1);
	}
	if (!cmd_strct->cmd_path && !builtin)
	{
		if (cmd_strct->cmd_array && !builtin)
			print_error_msg(cmd_strct->cmd_array[0], "command not found");
		g_exitcode = 127;
		return (1);
	}
	else if (cmd_strct->cmd_path && !builtin
		&& (!access(cmd_strct->cmd_path, F_OK)
			&& access(cmd_strct->cmd_path, X_OK) < 0))
	{
		if (cmd_strct->cmd_array[0] && !builtin)
			print_error_msg(cmd_strct->cmd_array[0], "permission denied");
		g_exitcode = 126;
		return (1);
	}
	return (0);
}

// start command execution
void	execute_single_builtin(t_uni *uni, t_cmmnds *cmd_strct)
{
	g_exitcode = exec_builtin(cmd_strct, isbuiltin(cmd_strct), 0);
	ft_lstiter(uni->cmd_lst, close_fds);
}

void	execute_commands(t_uni *uni)
{
	t_cmmnds	*cmd_strct;
	t_list		*current;

	current = uni->cmd_lst;
	while (current)
	{
		cmd_strct = (t_cmmnds *)current->content;
		logical_subshell(&cmd_strct, &current);
		if (cmd_strct && current)
		{
			run_cmmnds(cmd_strct);
			current = current->next;
		}
	}
	ft_lstiter(uni->cmd_lst, close_fds);
	wait_for_exitcode(uni);
}

void	executer(t_uni *uni)
{
	t_cmmnds	*cmd_strct;

	if (!uni->cmd_lst)
		return ;
	signal(SIGQUIT, &quit_handler);
	cmd_strct = (t_cmmnds *)uni->cmd_lst->content;
	if (ft_lstsize(uni->cmd_lst) == 1 && isbuiltin(cmd_strct))
		execute_single_builtin(uni, cmd_strct);
	else
		execute_commands(uni);
}
