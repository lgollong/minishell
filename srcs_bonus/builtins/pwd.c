/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:56:20 by lgollong          #+#    #+#             */
/*   Updated: 2024/12/10 15:30:50 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pwd(t_cmmnds *node)
{
	char	*str;

	str = getcwd(NULL, 0);
	if (!str)
		return (1);
	ft_putstr_fd(str, node->outf);
	ft_putstr_fd("\n", node->outf);
	free(str);
	return (0);
}
