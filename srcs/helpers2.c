/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 22:08:09 by tfriedri          #+#    #+#             */
/*   Updated: 2022/12/14 14:07:57 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// prints a long error-message
void	print_long_error_msg(char *cmmnd, char *arg, char *msg)
{
	if (!arg || !msg)
		return ;
	ft_putstr_fd("minishell: ", 2);
	if (cmmnd)
	{
		ft_putstr_fd(cmmnd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

// prints a typical error-message
void	print_error_msg(char *cmmnd, char *msg)
{
	if (!msg)
		return ;
	ft_putstr_fd("minishell: ", 2);
	if (cmmnd)
	{
		ft_putstr_fd(cmmnd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}
