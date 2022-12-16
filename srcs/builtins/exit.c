/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:57:09 by lgollong          #+#    #+#             */
/*   Updated: 2022/12/14 14:08:35 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	number_check(char *arg)
{
	size_t	i;

	i = 0;
	while (arg[i] >= 48 && arg[i] <= 57)
		i++;
	if (ft_strlen(arg) == i)
		return (0);
	return (1);
}

int	ft_exit(t_cmmnds *node)
{
	int	g_exitcode;

	g_exitcode = 0;
	if (array_length(node->cmd_array) > 2 && !number_check(node->cmd_array[1]))
		return (print_error_msg("exit", "too many arguments"), 1);
	ft_putstr_fd("exit\n", node->outf);
	if (array_length(node->cmd_array) > 1 && number_check(node->cmd_array[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(node->cmd_array[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
	}
	if (node->cmd_array[1])
		g_exitcode = ft_atoi(node->cmd_array[1]);
	exit_minishell(g_exitcode, node->uni);
	return (0);
}