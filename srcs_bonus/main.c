/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 14:51:48 by lgollong          #+#    #+#             */
/*   Updated: 2024/12/10 15:30:50 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_exitcode = 0;

// frees the uni->cmd_lst
void	clear_cmmnd_lst(void *content)
{
	t_cmmnds	*cmmnd_struct;

	cmmnd_struct = (t_cmmnds *)content;
	if (cmmnd_struct->cmd_path)
		free(cmmnd_struct->cmd_path);
	if (cmmnd_struct->cmd_array)
		clean_array(&cmmnd_struct->cmd_array);
	free(cmmnd_struct);
}

// free all allocated stuff
void	free_stuff(t_uni	*uni)
{
	if (uni->input_string)
		free(uni->input_string);
	if (uni->commands)
		clean_array(&uni->commands);
	if (uni->envp)
		clean_array(&uni->envp);
	ft_lstclear(&uni->cmd_lst, clear_cmmnd_lst);
	rl_clear_history();
	if (uni)
		free(uni);
}

// free and exit
void	exit_minishell(int exit_code, t_uni *uni)
{
	free_stuff(uni);
	exit(exit_code);
}

int	main(int argc, char *argv[], char **envp)
{
	t_uni	*uni;

	uni = init_uni_struct(argc, argv, envp);
	while (1)
	{
		parser(uni);
		executer(uni);
		free(uni->input_string);
		uni->pid = -1;
		uni->stop = 0;
		if (uni->commands)
			clean_array(&uni->commands);
		ft_lstclear(&uni->cmd_lst, clear_cmmnd_lst);
	}
	return (0);
}
