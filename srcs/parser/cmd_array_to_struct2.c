/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_to_struct2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:55:11 by rwegat            #+#    #+#             */
/*   Updated: 2025/01/16 16:55:53 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check if string is empty
int	is_empty(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] != ' ')
			return (1);
	}
	return (0);
}

// gets redirection and direction types
// type == 0 ?		command
// type == 1 ?		infile-redirection
// type == 2 ?		here_doc-redirection
// type == 3 ?		write to outfile
// type == 4 ?		append to outfile
// type == 5 ?		pipe
// bonus:
// type == 6 ?		&& AND
// type == 7 ?		|| OR
// type == 8 ?		( parentheses open
// type == 9 ?		) parentheses close
int	get_redirection_type(t_uni *uni, int i)
{
	int	type;

	type = 0;
	if (ft_strlen(uni->commands[i]) == 1
		&& ft_strcmp(uni->commands[i], "<") == 0)
		type = INFILE_RE;
	if (type == INFILE_RE && uni->commands[i + 1]
		&& ft_strlen(uni->commands[i + 1]) == 1
		&& ft_strcmp(uni->commands[i + 1], "<") == 0)
		type = HERE_DOC_RE;
	if (type == 0 && ft_strlen(uni->commands[i]) == 1
		&& ft_strcmp(uni->commands[i], ">") == 0)
		type = WR_TO_OUT;
	if (type == WR_TO_OUT && uni->commands[i + 1]
		&& ft_strlen(uni->commands[i + 1]) == 1
		&& ft_strcmp(uni->commands[i + 1], ">") == 0)
		type = AP_TO_OUT;
	return (type);
}

int	get_type(t_uni *uni, int i)
{
	int	type;

	type = get_redirection_type(uni, i);
	if (type == 0 && ft_strcmp(uni->commands[i], "|") == 0)
		type = PIPE;
	if (type == 0 && ft_strlen(uni->commands[i]) == 2
		&& ft_strcmp(uni->commands[i], "&&") == 0)
		type = AND;
	if (type == 0 && ft_strlen(uni->commands[i]) == 2
		&& ft_strcmp(uni->commands[i], "||") == 0)
		type = OR;
	if (type == 0 && ft_strlen(uni->commands[i]) == 1
		&& ft_strcmp(uni->commands[i], "(") == 0)
		type = PAR_OPEN;
	if (type == 0 && ft_strlen(uni->commands[i]) == 1
		&& ft_strcmp(uni->commands[i], ")") == 0)
		type = PAR_CLOSE;
	return (type);
}

// returns a new t_cmmnds struct
t_cmmnds	*create_cmmnd_struct(t_uni *uni)
{
	t_cmmnds	*cmmnd_struct;

	cmmnd_struct = ft_calloc(1, sizeof(t_cmmnds));
	if (!cmmnd_struct)
		return (NULL);
	cmmnd_struct->cmd_array = NULL;
	cmmnd_struct->cmd_path = NULL;
	cmmnd_struct->inf = 0;
	cmmnd_struct->outf = 1;
	cmmnd_struct->uni = uni;
	cmmnd_struct->broken = 0;
	cmmnd_struct->left = NULL;
	cmmnd_struct->right = NULL;
	cmmnd_struct->scope = uni->scope_p;
	cmmnd_struct->type = 0;
	return (cmmnd_struct);
}
