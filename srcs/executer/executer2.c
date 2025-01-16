/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:51:11 by rwegat            #+#    #+#             */
/*   Updated: 2025/01/16 16:52:53 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// iterates through the t_list and runs all commands
void	run_builtin_or_fork(t_cmmnds *cmd_strct, int builtin)
{
	if (!builtin || cmd_strct->type == PIPE || \
	(cmd_strct->left && cmd_strct->left->type == PIPE))
	{
		cmd_strct->uni->pid = fork();
		if (cmd_strct->uni->pid < 0)
			return ;
		if (cmd_strct->uni->pid == 0)
		{
			if (builtin)
				exec_builtin(cmd_strct, builtin, 1);
			dup2(cmd_strct->inf, 0);
			dup2(cmd_strct->outf, 1);
			ft_lstiter(cmd_strct->uni->cmd_lst, close_fds);
			execve(cmd_strct->cmd_path, cmd_strct->cmd_array, \
			cmd_strct->uni->envp);
			exit_minishell(2, cmd_strct->uni);
		}
	}
	else
		g_exitcode = exec_builtin(cmd_strct, builtin, 0);
}

void	run_cmmnds(void *content)
{
	t_cmmnds	*cmd_strct;
	int			builtin;

	cmd_strct = (t_cmmnds *)content;
	if (cmd_strct->uni->stop == 1)
		return ;
	builtin = isbuiltin(cmd_strct);
	if (is_not_executable(cmd_strct, builtin) == 1)
		return ;
	run_builtin_or_fork(cmd_strct, builtin);
}

// waits for subprocesses and ensures correct exit codes
void	wait_for_exitcode(t_uni *uni)
{
	int			wstatus;
	int			temp_exitcode;
	t_cmmnds	*last;

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
}

// skips commands that are not supposed to be executed (&& and ||)
void	handle_scope(t_cmmnds **tmp, t_list **current, int left_scope)
{
	if ((*tmp)->left->type == OR && !g_exitcode)
	{
		while ((*tmp) && (*tmp)->scope > left_scope)
		{
			*tmp = (*tmp)->right;
			*current = (*current)->next;
		}
	}
	else if ((*tmp)->left->type == AND && g_exitcode)
	{
		while ((*tmp) && (*tmp)->scope > left_scope)
		{
			*tmp = (*tmp)->right;
			*current = (*current)->next;
		}
	}
}

void	logical_subshell(t_cmmnds **tmp, t_list **current)
{
	int		left_scope;

	left_scope = 0;
	if (!(*tmp)->left)
		return ;
	if ((*tmp)->left->scope != (*tmp)->scope)
	{
		left_scope = (*tmp)->left->scope;
		handle_scope(tmp, current, left_scope);
		if (!*tmp)
			return ;
	}
	while (*tmp && (*tmp)->left && (*tmp)->left->type == AND && g_exitcode)
	{
		*tmp = (*tmp)->right;
		*current = (*current)->next;
	}
	while (*tmp && (*tmp)->left && (*tmp)->left->type == OR && !g_exitcode)
	{
		*tmp = (*tmp)->right;
		*current = (*current)->next;
	}
}
