/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 11:24:36 by lgollong          #+#    #+#             */
/*   Updated: 2024/12/10 15:30:50 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// prints an syntax-error-message and sets uni->stop to 1
int	syntax_error(t_uni *uni, char *token)
{
	uni->stop = 1;
	if (!token)
		print_error_msg(NULL, "syntax error near unexpected token `newline'");
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token, 2);
		ft_putstr_fd("'\n", 2);
	}
	return (0);
}

// opens the file from commands[i] 
int	open_file(t_uni *uni, int i, int *save_fd, int flags[2])
{
	if (*save_fd > 1)
	{
		if (close(*save_fd) == -1)
			perror("minishell");
	}
	if (uni->commands[i + 1] != NULL && (uni->commands[i + 1][0] != '>'
		&& uni->commands[i + 1][0] != '<'))
	{
		if (flags[1] != 0)
			*save_fd = open(uni->commands[i + 1], flags[0], flags[1]);
		else
			*save_fd = open(uni->commands[i + 1], flags[0]);
		if (*save_fd == -1)
		{
			print_error_msg(uni->commands[i + 1], "No such file or directory");
			return (1);
		}
	}
	else
		syntax_error(uni, uni->commands[i + 1]);
	return (0);
}

// gets flags for file-opening
int	get_flags(int type, int arr_pos)
{
	if (arr_pos == 0)
	{
		if (type == 1 || type == 2)
			return (O_RDONLY);
		if (type == 3)
			return (O_WRONLY | O_CREAT | O_TRUNC);
		if (type == 4)
			return (O_WRONLY | O_CREAT | O_APPEND);
	}
	else if (arr_pos == 1)
	{
		if (type == 1 || type == 2)
			return (0);
		if (type == 3 || type == 4)
			return (00644);
	}
	return (0);
}

// cleans the commands-array from <, <<, >, >>, infiles and outfiles
void	del_inf_and_outf(int type, t_uni *uni, int i)
{
	if (type == 1 || type == 3)
		del_str_from_array(uni->commands, i, 2);
	else if (type == 2 || type == 4)
		del_str_from_array(uni->commands, i, 3);
}

int	open_file_and_save_fd(t_uni *uni, t_cmmnds *cmd_strct, int i, int type)
{
	int	flags[2];
	int	ret_val;

	flags[0] = get_flags(type, 0);
	flags[1] = get_flags(type, 1);
	if (type == 1)
		cmd_strct->broken = open_file(uni, i, &cmd_strct->inf, flags);
	else if (type == 2)
		start_here_doc(uni, cmd_strct, i);
	else if (type == 3)
		cmd_strct->broken = open_file(uni, i, &cmd_strct->outf, flags);
	else
		cmd_strct->broken = open_file(uni, i + 1, &cmd_strct->outf, flags);
	if (type == 2 || type == 4)
		ret_val = 3;
	else
		ret_val = 2;
	return (ret_val);
}
