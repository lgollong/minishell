/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/06 15:56:20 by lgollong          #+#    #+#             */
/*   Updated: 2022/12/09 01:32:57 by tfriedri         ###   ########.fr       */
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
