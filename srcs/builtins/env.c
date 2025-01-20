/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:57:14 by lgollong          #+#    #+#             */
/*   Updated: 2022/12/09 01:26:52 by tfriedri         ###   ########.fr       */
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
