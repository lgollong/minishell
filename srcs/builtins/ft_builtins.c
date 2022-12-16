/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:53:38 by lgollong          #+#    #+#             */
/*   Updated: 2022/12/13 11:30:34 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	isbuiltin(t_cmmnds *node)
{
	if (!node->cmd_array)
		return (0);
	if (!ft_strcmp(node->cmd_array[0], "echo"))
		return (1);
	else if (!ft_strcmp(node->cmd_array[0], "pwd"))
		return (2);
	if (!ft_strcmp(node->cmd_array[0], "env"))
		return (3);
	else if (!ft_strcmp(node->cmd_array[0], "unset"))
		return (4);
	else if (!ft_strcmp(node->cmd_array[0], "cd"))
		return (5);
	else if (!ft_strcmp(node->cmd_array[0], "export"))
		return (6);
	else if (!ft_strcmp(node->cmd_array[0], "exit"))
		return (7);
	return (0);
}

int	exec_builtin(t_cmmnds *node, int builtin, int forked)
{
	int	ret_val;

	if (builtin == 1)
		ret_val = ft_echo(node);
	else if (builtin == 2)
		ret_val = ft_pwd(node);
	else if (builtin == 3)
		ret_val = ft_env(node);
	else if (builtin == 4)
		ret_val = ft_unset(node);
	else if (builtin == 5)
		ret_val = ft_cd(node);
	else if (builtin == 6)
		ret_val = ft_export(node);
	else if (builtin == 7)
		ret_val = ft_exit(node);
	else
		ret_val = 127;
	if (forked == 1)
	{
		ft_lstiter(node->uni->cmd_lst, close_fds);
		exit_minishell(ret_val, node->uni);
	}
	return (ret_val);
}

char	**change_in_env(char *set, char *replace, char **envp)
{
	int		i;

	i = 0;
	while (envp && replace && envp[i])
	{
		if (!ft_strncmp(envp[i], set, ft_strlen(set)))
		{
			free(envp[i]);
			envp[i] = ft_strjoin(set, replace);
		}
		i++;
	}
	if (replace)
		free(replace);
	return (envp);
}
