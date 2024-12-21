/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 14:16:32 by rwegat            #+#    #+#             */
/*   Updated: 2024/12/21 17:20:34 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdio.h>

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

// iterates throught the t_list and runs all commands
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
	if (!builtin || (cmd_strct && cmd_strct->type == PIPE))
	{
		cmd_strct->uni->pid = fork();
		if (cmd_strct->uni->pid < 0)
			return ;
		if (cmd_strct->uni->pid == 0)
		{
			printf("\tChild process running with PID: %d, Command: %s, Arguments: ", getpid(), cmd_strct->cmd_array ? cmd_strct->cmd_array[0] : "NULL");
			for (int i = 1; cmd_strct->cmd_array && cmd_strct->cmd_array[i]; i++)
				printf("%s ", cmd_strct->cmd_array[i]);
			printf("\n");
			if (builtin)
				exec_builtin(cmd_strct, builtin, 1);
			dup2(cmd_strct->inf, 0);
			dup2(cmd_strct->outf, 1);
			ft_lstiter(cmd_strct->uni->cmd_lst, close_fds);
			execve(cmd_strct->cmd_path, cmd_strct->cmd_array,
				cmd_strct->uni->envp);	
			exit_minishell(2, cmd_strct->uni);
		}
		else
		{
			int wstatus;
			printf("\tParent process continues with PID: %d, Command: %s, Arguments: ", getpid(), cmd_strct->cmd_array ? cmd_strct->cmd_array[0] : "NULL");
			for (int i = 1; cmd_strct->cmd_array && cmd_strct->cmd_array[i]; i++)
				printf("%s ", cmd_strct->cmd_array[i]);
			printf("\n");
			if (cmd_strct && cmd_strct->type == PIPE)
			{
				printf("\tWaiting for pipe, Command: %s, Arguments: ", cmd_strct->cmd_array ? cmd_strct->cmd_array[0] : "NULL");
				for (int i = 1; cmd_strct->cmd_array && cmd_strct->cmd_array[i]; i++)
					printf("%s ", cmd_strct->cmd_array[i]);
				printf("\n");
				if (waitpid(cmd_strct->uni->pid, &wstatus, WNOHANG) == -1)
					return;
			}
			// else if ((cmd_strct && (cmd_strct->type == AND || cmd_strct->type == OR)))
			// {
			// 	printf("\tWaiting for logical, Command: %s, Arguments: ", cmd_strct->cmd_array ? cmd_strct->cmd_array[0] : "NULL");
			// 	for (int i = 1; cmd_strct->cmd_array && cmd_strct->cmd_array[i]; i++)
			// 		printf("%s ", cmd_strct->cmd_array[i]);
			// 	printf("\n");
			// 	if (waitpid(cmd_strct->uni->pid, &wstatus, 0) == -1)
			// 		return;
			// }
			if (WIFEXITED(wstatus))
			{
				g_exitcode = WEXITSTATUS(wstatus);
				printf("\tChild process with PID: %d, Command: %s, Arguments: ", cmd_strct->uni->pid, cmd_strct->cmd_array ? cmd_strct->cmd_array[0] : "NULL");
				for (int i = 1; cmd_strct->cmd_array && cmd_strct->cmd_array[i]; i++)
					printf("%s ", cmd_strct->cmd_array[i]);
				printf("exited with code: %d\n", g_exitcode);
			}
		}
	}
	else
	{
		printf("\tExecuting builtin, Command: %s, Arguments: ", cmd_strct->cmd_array ? cmd_strct->cmd_array[0] : "NULL");
		for (int i = 1; cmd_strct->cmd_array && cmd_strct->cmd_array[i]; i++)
			printf("%s ", cmd_strct->cmd_array[i]);
		printf("\n");
		g_exitcode = exec_builtin(cmd_strct, builtin, 0);
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
	// if (waitpid(uni->pid, &wstatus, 0) != -1 && WIFEXITED(wstatus))
	// 	temp_exitcode = WEXITSTATUS(wstatus);
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

// skips commands that are not supposed to be executed (&& and ||)
// skips commands of higher scope
void	logical_subshell(t_cmmnds **tmp, t_list **current)
{
	int		left_scope;
	(void)current;

	left_scope = 0;
	if (!(*tmp)->left)
		return ;
	if ((*tmp)->left->scope != (*tmp)->scope)
	{
		left_scope = (*tmp)->left->scope;
		if ((*tmp)->left->type == OR && !g_exitcode)
		{
			while ((*tmp) && (*tmp)->scope > left_scope){
				*tmp = (*tmp)->right;
				*current = (*current)->next;
			}
		}
		else if ((*tmp)->left->type == AND && g_exitcode)
		{
			while ((*tmp) && (*tmp)->scope > left_scope){
				*tmp = (*tmp)->right;
				*current = (*current)->next;
			}
		}
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

// start command execution
void	executer(t_uni *uni)
{
	t_cmmnds	*cmd_strct;
	t_list		*current;

	if (!uni->cmd_lst)
		return ;
	signal(SIGQUIT, &quit_handler);
	cmd_strct = (t_cmmnds *)uni->cmd_lst->content;
	t_list *temp = uni->cmd_lst;
	while (temp)
	{
		t_cmmnds *cmd = (t_cmmnds *)temp->content;
		printf("\tCommand: %s, Type: %d", cmd->cmd_array ? cmd->cmd_array[0] : "NULL", cmd->type);
		if (cmd->left)
			printf(", Left Type: %d", cmd->left->type);
		else
			printf(", Left Type: NULL");
		if (cmd->right)
			printf(", Right Type: %d", cmd->right->type);
		else
			printf(", Right Type: NULL");
		printf("\n");
		temp = temp->next;
	}
	printf("\tParent process running with PID: %d\n", getpid());
	if (ft_lstsize(uni->cmd_lst) == 1 && isbuiltin(cmd_strct))
	{
		g_exitcode = exec_builtin(cmd_strct, isbuiltin(cmd_strct), 0);
		ft_lstiter(uni->cmd_lst, close_fds);
	}
	else
	{
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
}
