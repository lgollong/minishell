/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_executer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 23:43:19 by rwegat            #+#    #+#             */
/*   Updated: 2024/12/11 11:50:02 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// iterates throught the t_list and closes all open file-descriptors
void	close_fds(void *content)
{
	t_cmmnds	*cmd_strct;

	cmd_strct = (t_cmmnds *)content;
	if (cmd_strct->inf != 0 && cmd_strct->inf != -1)
		close(cmd_strct->inf);
	if (cmd_strct->outf != 1 && cmd_strct->outf != -1)
		close(cmd_strct->outf);
}

// checks if command is executable
int	is_not_executable(t_cmmnds	*cmd_strct, int builtin)
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
void print_cmmnd_details(t_cmmnds *cmd)
{
	printf("Command Array: ");
	if (cmd->cmd_array)
	{
		for (int i = 0; cmd->cmd_array[i]; i++)
		{
			printf("%s ", cmd->cmd_array[i]);
		}
	}
	printf("\n");
	printf("Command Path: %s\n", cmd->cmd_path ? cmd->cmd_path : "NULL");
	printf("Input FD: %d\n", cmd->inf);
	printf("Output FD: %d\n", cmd->outf);
	printf("Broken: %d\n", cmd->broken);
	printf("Type: %d\n", cmd->type);
	printf("Exit Status: %d\n", cmd->exit_status);
	printf("Scope: %d\n", cmd->scope);
	printf("Left: %p\n", (void *)cmd->left);
	printf("Right: %p\n", (void *)cmd->right);
	printf("--------------------\n");
}

// 
void	logical_subshell(t_uni *uni, t_cmmnds **tmp)
{
	int		left_scope;

	left_scope = 0;
	if (!(*tmp)->left)
		return ;
	if ((*tmp)->left->scope != (*tmp)->scope)
	{
		left_scope = (*tmp)->left->scope;
		if ((*tmp)->left->type == OR && uni->last_exit_status == 0)
		{
			while ((*tmp) && (*tmp)->scope > left_scope)
				*tmp = (*tmp)->right;
		}
		else if ((*tmp)->left->type == AND && uni->last_exit_status != 0)
		{
			while ((*tmp) && (*tmp)->scope > left_scope)
				*tmp = (*tmp)->right;
		}
		if (!*tmp)
			return ;
	}
	while (*tmp && (*tmp)->left->type == AND)
		*tmp = (*tmp)->right;
	while (*tmp && (*tmp)->left->type == OR)
		*tmp = (*tmp)->right;
}

void	exec_real_command(t_uni *uni, t_cmmnds **temp)
{
	int		fpipe[2];

	logical_subshell(uni, *temp);
	if (!*temp)
		return ;
	
}

// iterates throught the t_list and runs all commands
void	run_cmmnds(t_uni *uni)
{
	t_cmmnds	*temp;

	temp = uni->cmd_lst;
	uni->scope_p = 0;
	while (temp)
	{
		if (!temp)
			break ;
		print_cmmnd_details(temp);
		exec_real_command(uni, &temp);
		if (!temp)
			break ;
		temp = temp->right;
	}
}

// wait for the subprocesses and gets exitcode
void	wait_for_exitcode(t_uni *uni)
{
	int			wstatus;
	t_cmmnds	*last;
	int			temp_exitcode;

	temp_exitcode = -1;
	last = ft_lstlast(uni->cmd_lst)->content;
	if (waitpid(uni->pid, &wstatus, 0) != -1 && WIFEXITED(wstatus))
		temp_exitcode = WEXITSTATUS(wstatus);
	while ((waitpid(-1, &wstatus, 0) != -1))
		continue ;
	if (temp_exitcode != -1)
		g_exitcode = temp_exitcode;
	if (g_exitcode == 130)
		write(2, "\n", 1);
	if (g_exitcode == 131)
		write(2, "Quit\n", 5);
	if (last->broken != 0)
		g_exitcode = last->broken;
	else if (!isbuiltin(last) && !last->cmd_path)
		g_exitcode = 127;
	else if (last->cmd_path && !isbuiltin(last)
		&& (!access(last->cmd_path, F_OK)
			&& access(last->cmd_path, X_OK) < 0))
		g_exitcode = 126;
	return ;
}

// start command execution
void	executer(t_uni *uni)
{
	t_cmmnds	*cmd_strct;
	int			builtin;

	if (!uni->cmd_lst)
		return ;
	signal(SIGQUIT, &quit_handler);
	cmd_strct = (t_cmmnds *)uni->cmd_lst->content;
	builtin = isbuiltin(cmd_strct);
	if (ft_lstsize(uni->cmd_lst) == 1 && builtin)
	{
		g_exitcode = exec_builtin(cmd_strct, builtin, 0);
		ft_lstiter(uni->cmd_lst, close_fds);
	}
	else
	{
		run_cmmnds(uni);
		ft_lstiter(uni->cmd_lst, close_fds);
		wait_for_exitcode(uni);
	}
}
