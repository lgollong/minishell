/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:57:14 by lgollong          #+#    #+#             */
/*   Updated: 2024/12/10 15:30:50 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_env(t_cmmnds *node)
{
	int	i;

	i = 0;
	if (!node->uni->envp)
		return (1);
	while (node->uni->envp[i])
	{
		if (ft_strchr(node->uni->envp[i], '='))
		{
			ft_putstr_fd(node->uni->envp[i], node->outf);
			ft_putstr_fd("\n", node->outf);
			i++;
		}
		else
			i++;
	}
	return (0);
}
