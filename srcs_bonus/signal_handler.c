/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 08:24:29 by tilman            #+#    #+#             */
/*   Updated: 2022/12/17 17:33:55 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// signal-handler for strg+c while readline
void	sig_handler_rl(int num)
{
	if (num == SIGINT)
	{
		g_exitcode = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

// signal-handler for strg+c after readline
void	sig_handler_after_rl(int num)
{
	if (num == SIGINT)
		g_exitcode = 130;
}

// signal-handler while here_doc
void	sig_handler_here_doc(int num)
{
	if (num == SIGINT)
	{
		g_exitcode = 1;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

// signal handler for strg+\ while execution
void	quit_handler(int num)
{
	if (num == SIGQUIT)
		g_exitcode = 131;
}
