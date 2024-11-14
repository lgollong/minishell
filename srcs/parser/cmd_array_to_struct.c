/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_to_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 17:04:53 by tfriedri          #+#    #+#             */
/*   Updated: 2024/11/14 12:41:13 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// gets redirection and direction types
// type == 0 ?		command
// type == 1 ?		infile-redirection
// type == 2 ?		here_doc-redirection
// type == 3 ?		write to outfile
// type == 4 ?		append to outfile
// type == 5 ?		pipe
int	get_type(t_uni *uni, int i)
{
	int	type;

	type = 0;
	if (ft_strlen(uni->commands[i]) == 1
		&& ft_strcmp(uni->commands[i], "<") == 0)
		type = 1;
	if (type == 1 && uni->commands[i + 1]
		&& ft_strlen(uni->commands[i + 1]) == 1
		&& ft_strcmp(uni->commands[i + 1], "<") == 0)
		type = 2;
	if (type == 0 && ft_strlen(uni->commands[i]) == 1
		&& ft_strcmp(uni->commands[i], ">") == 0)
		type = 3;
	if (type == 3 && uni->commands[i + 1]
		&& ft_strlen(uni->commands[i + 1]) == 1
		&& ft_strcmp(uni->commands[i + 1], ">") == 0)
		type = 4;
	if (type == 0 && ft_strcmp(uni->commands[i], "|") == 0)
		type = 5;
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
	return (cmmnd_struct);
}

// appends a new t_cmmnds struct to the cmmnds_lst
t_list	*next_cmmnd_struct(t_uni *uni, t_list *last, int i)
{
	t_list	*tmp;
	int		*tube;

	tmp = last;
	if (!uni->commands[i + 1] && ft_strcmp(uni->commands[i], "|"))
		syntax_error(uni, uni->commands[i + 1]);
	ft_lstadd_back(&uni->cmd_lst, ft_lstnew(create_cmmnd_struct(uni)));
	last = ft_lstlast(uni->cmd_lst);
	if (!last || !last->content)
		exit(1);
	tube = ft_calloc(2, sizeof(int));
	if (!tube)
		return (NULL);
	if (pipe(tube) == -1)
	{
		free(tube);
		return (NULL);
	}
	if (((t_cmmnds *)tmp->content)->outf == 1)
		((t_cmmnds *)tmp->content)->outf = tube[1];
	else
		close(tube[1]);
	((t_cmmnds *)last->content)->inf = tube[0];
	free(tube);
	return (last);
}

// copies the command to the cmmnd_struct
void	cmmnd_to_struct(t_cmmnds *cmmnd_struct, char *command)
{
	(cmmnd_struct)->cmd_array = add_str_to_array(
			(cmmnd_struct)->cmd_array, command);
}

// loops throught the commands-array and creates the 
// cmd_lst with t_cmmnds-structs as content
void	cmd_array_to_struct(t_uni *uni)
{
	int		i;
	int		type;
	t_list	*last;

	i = 0;
	uni->cmd_lst = ft_lstnew(create_cmmnd_struct(uni));
	last = uni->cmd_lst;
	while (uni->stop == 0 && uni->commands[i]
		&& last != NULL && last->content != NULL)
	{
		type = get_type(uni, i);
		if (type > 0 && type < 5 && ((t_cmmnds *)last->content)->broken == 0)
			i = i + open_file_and_save_fd(uni, last->content, i, type);
		else if (type == 0 && ((t_cmmnds *)last->content)->broken == 0)
		{
			if (handle_wildcard(uni->commands[i], (t_cmmnds *)last->content))
				cmmnd_to_struct(((t_cmmnds *)last->content), uni->commands[i]);
		}
		else if (type == 5)
			last = next_cmmnd_struct(uni, last, i);
		i++;
	}
	if (i != 0 && i == array_length(uni->commands) && (uni->commands[i - 1]
			&& !ft_strcmp(uni->commands[i - 1], "|")))
		syntax_error(uni, "|");
}
