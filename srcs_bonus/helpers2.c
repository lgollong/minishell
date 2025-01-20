/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 22:08:09 by lgollong          #+#    #+#             */
/*   Updated: 2024/12/10 15:30:50 by lgollong         ###   ########.fr       */
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
