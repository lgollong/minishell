#include "../../minishell.h"

// Closes all open file descriptors in the command list
void close_fds(void *content)
{
	t_cmmnds *cmd_strct = (t_cmmnds *)content;
	if (cmd_strct->inf != 0 && cmd_strct->inf != -1)
	{
		close(cmd_strct->inf);
		cmd_strct->inf = -1;
	}
	if (cmd_strct->outf != 1 && cmd_strct->outf != -1)
	{
		close(cmd_strct->outf);
		cmd_strct->outf = -1;
	}
}

// Executes commands based on type and builtin status
void exec_commands2(t_uni *uni, t_cmmnds *cmd_strct)
{
	int builtin = isbuiltin(cmd_strct);

	if (isbuiltin(cmd_strct) && cmd_strct->cmd_array)
		exec_builtin(cmd_strct, builtin, 0);
	else if (!cmd_strct->cmd_path)
	{
		if (cmd_strct->cmd_array)
			print_error_msg(cmd_strct->cmd_array[0], "command not found");
		g_exitcode = 127;
	}
	else if (cmd_strct->cmd_path && !builtin && access(cmd_strct->cmd_path, F_OK) == 0 && access(cmd_strct->cmd_path, X_OK) < 0)
	{
		print_error_msg(cmd_strct->cmd_array[0], "permission denied");
		g_exitcode = 126;
	}
	else if (cmd_strct->cmd_path && !builtin)
		execve(cmd_strct->cmd_path, cmd_strct->cmd_array, uni->envp);
}

// Executes a real command, handling pipes and chaining
void exec_real_command(t_uni *uni, t_cmmnds **cmd_strct)
{
	int fpipe[2];

	if (!*cmd_strct)
		return;
	if (pipe(fpipe) < 0)
		return;
	exec_commands2(uni, *cmd_strct);
	close(fpipe[0]);
	close(fpipe[1]);
	while ((*cmd_strct)->type == PIPE)
		*cmd_strct = (*cmd_strct)->right;
}

// Main command execution loop
void exec_commands(t_uni *uni)
{
	t_cmmnds *cmd_strct;

	cmd_strct = (t_cmmnds *)uni->cmd_lst->content;
	while (cmd_strct)
	{
		exec_real_command(uni, &cmd_strct);
		if (!cmd_strct)
			break;
		cmd_strct = cmd_strct->right;
	}
}

// Determines if a command is executable
int is_not_executable(t_cmmnds *cmd_strct, int builtin)
{
	if (cmd_strct->broken != 0)
	{
		g_exitcode = cmd_strct->broken;
		return (1);
	}
	if (!cmd_strct->cmd_path && !builtin)
	{
		if (cmd_strct->cmd_array)
			print_error_msg(cmd_strct->cmd_array[0], "command not found");
		g_exitcode = 127;
		return (1);
	}
	if (cmd_strct->cmd_path && !builtin && access(cmd_strct->cmd_path, X_OK) < 0)
	{
		print_error_msg(cmd_strct->cmd_array[0], "permission denied");
		g_exitcode = 126;
		return (1);
	}
	return (0);
}

// Executes commands in a command list
void run_cmmnds(void *content)
{
	t_cmmnds *cmd_strct = (t_cmmnds *)content;
	int builtin = isbuiltin(cmd_strct);
	int wstatus;

	if (cmd_strct->uni->stop == 1)
		return ;
	if (is_not_executable(cmd_strct, builtin) == 1)
		return ;
	if (!builtin || (cmd_strct->left && cmd_strct->left->type == PIPE))
	{
		cmd_strct->uni->pid = fork();
		if (cmd_strct->uni->pid < 0)
			return ;
		if (cmd_strct->uni->pid == 0)
		{
			if (builtin) exec_builtin(cmd_strct, builtin, 1);
			dup2(cmd_strct->inf, 0);
			dup2(cmd_strct->outf, 1);
			ft_lstiter(cmd_strct->uni->cmd_lst, close_fds);
			execve(cmd_strct->cmd_path, cmd_strct->cmd_array, cmd_strct->uni->envp);
			exit_minishell(2, cmd_strct->uni);
		}
		else
		{
			if (cmd_strct->left && cmd_strct->left->type == PIPE)
			{
				while (waitpid(cmd_strct->uni->pid, &wstatus, 0) > 0)
					continue ;
			}
			else if (cmd_strct->type == AND || cmd_strct->type == OR)
			{
				if (waitpid(cmd_strct->uni->pid, &wstatus, 0) == -1)
					return ;
			}
			if (WIFEXITED(wstatus))
				g_exitcode = WEXITSTATUS(wstatus);
		}
	}
	else
		g_exitcode = exec_builtin(cmd_strct, builtin, 0);
}

// Logical subshell handling for AND/OR
void logical_subshell(t_cmmnds **tmp, t_list **current)
{
	int left_scope;

	if (!(*tmp)->left) return;

	if ((*tmp)->left->scope != (*tmp)->scope)
	{
		left_scope = (*tmp)->left->scope;
		if ((*tmp)->left->type == OR && !g_exitcode)
		{
			while (*tmp && (*tmp)->scope > left_scope)
			{
				*tmp = (*tmp)->right;
				*current = (*current)->next;
			}
		}
		else if ((*tmp)->left->type == AND && g_exitcode)
		{
			while (*tmp && (*tmp)->scope > left_scope)
			{
				*tmp = (*tmp)->right;
				*current = (*current)->next;
			}
		}
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

// Waits for subprocesses and gets the exit code
void wait_for_exitcode(t_uni *uni)
{
	int wstatus;
	t_cmmnds *last;
	int temp_exitcode;

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

// Main executor entry point
void executer(t_uni *uni)
{
	t_cmmnds *cmd_strct;
	t_list *current;

	if (!uni->cmd_lst)
		return;
	signal(SIGQUIT, &quit_handler);
	cmd_strct = (t_cmmnds *)uni->cmd_lst->content;
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
