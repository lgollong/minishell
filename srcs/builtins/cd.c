/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:57:22 by lgollong          #+#    #+#             */
/*   Updated: 2022/12/17 19:09:21 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_args(t_cmmnds *node, char	*check)
{
	char	*strs[2];

	if (!check)
	{
		strs[0] = get_own_env(node->uni, "HOME");
		if (!strs[0])
			return (print_long_error_msg("cd", "HOME", "not set"), 1);
		chdir(strs[0]);
		node->uni->envp = change_in_env("PWD=",
				getcwd(NULL, 0), node->uni->envp, 0);
		free(strs[0]);
	}
	else if (!ft_strcmp(check, "-"))
	{
		strs[1] = get_own_env(node->uni, "OLDPWD");
		if (!strs[1])
			return (print_long_error_msg("cd", "OLDPWD", "not set"), 1);
		printf("%s\n", strs[1]);
		chdir(strs[1]);
		node->uni->envp = change_in_env("PWD=",
				getcwd(NULL, 0), node->uni->envp, 0);
		free(strs[1]);
	}
	return (0);
}

int	ft_cd(t_cmmnds *node)
{
	DIR	*dir;

	if (node->cmd_array && (!node->cmd_array[1]
			|| !ft_strcmp(node->cmd_array[1], "-")))
		return (check_args(node, node->cmd_array[1]));
	node->uni->envp = change_in_env("OLDPWD=",
			getcwd(NULL, 0), node->uni->envp, 0);
	dir = opendir(node->cmd_array[1]);
	if (!dir || chdir(node->cmd_array[1]) < 0)
		return (print_long_error_msg(node->cmd_array[0],
				node->cmd_array[1], "No such file or directory"), 1);
	closedir(dir);
	node->uni->envp = change_in_env("PWD=", getcwd(NULL, 0),
			node->uni->envp, 0);
	return (0);
}
