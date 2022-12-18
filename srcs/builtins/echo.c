/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:57:18 by lgollong          #+#    #+#             */
/*   Updated: 2022/12/18 16:42:27 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	check_newline(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-')
		return (1);
	while (str[i])
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	if (i == 1)
		return (1);
	return (0);
}

int	ft_echo(t_cmmnds *node)
{
	int	i;
	int	nl;

	nl = 0;
	i = 1;
	if (!node)
		return (1);
	if (!node->cmd_array[i])
		return (ft_putstr_fd("\n", node->outf), 1);
	while (!check_newline(node->cmd_array[i]))
	{
		nl++;
		i++;
	}
	i = 1 + nl;
	while (node->cmd_array[i])
	{
		ft_putstr_fd(node->cmd_array[i], node->outf);
		i++;
		if (node->cmd_array[i])
			ft_putstr_fd(" ", node->outf);
	}
	if (!nl)
		ft_putstr_fd("\n", node->outf);
	return (0);
}
