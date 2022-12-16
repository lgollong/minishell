/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:56:54 by lgollong          #+#    #+#             */
/*   Updated: 2022/12/06 15:59:11 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

size_t	env_len(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0' && str[i] != '=')
	{
		i++;
	}
	return (i);
}

int	ft_unset(t_cmmnds *node)
{
	t_uni	*u;
	int		i[2];

	i[0] = 0;
	u = node->uni;
	if (!u->envp || !node->cmd_array)
		return (1);
	while (u->envp[i[0]])
	{
		i[1] = 1;
		while (node->cmd_array[i[1]] && ft_strcmp(node->cmd_array[i[1]], "_"))
		{
			if (!ft_strncmp(u->envp[i[0]], node->cmd_array[i[1]],
					env_len(u->envp[i[0]])) && env_len(
					u->envp[i[0]]) == ft_strlen(node->cmd_array[i[1]]))
				del_str_from_array(u->envp, i[0], 1);
			i[1]++;
		}
		i[0]++;
	}
	return (0);
}
