/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_to_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 14:15:52 by rwegat            #+#    #+#             */
/*   Updated: 2025/01/16 16:54:37 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// appends a new t_cmmnds struct to the cmmnds_lst
void	set_pipes(t_cmmnds *tmp_content, t_cmmnds *last_content, int *tube)
{
	if (tmp_content->type == PIPE)
		last_content->inf = tube[0];
	else
		close(tube[0]);
	if (tmp_content->type == PIPE)
		tmp_content->outf = tube[1];
	else
		close(tube[1]);
}

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
	((t_cmmnds *)tmp->content)->right = (t_cmmnds *)last->content;
	((t_cmmnds *)last->content)->left = (t_cmmnds *)tmp->content;
	tube = ft_calloc(2, sizeof(int));
	if (!tube)
		return (NULL);
	if (pipe(tube) == -1)
	{
		free(tube);
		return (NULL);
	}
	set_pipes((t_cmmnds *)tmp->content, (t_cmmnds *)last->content, tube);
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
void	handle_command_type(t_uni *uni, t_list **last, int *i, int type)
{
	if (type == PAR_OPEN)
		uni->scope_p++;
	else if (type == PAR_CLOSE)
		uni->scope_p--;
	if (type > 0 && type < 5 && ((t_cmmnds *)(*last)->content)->broken == 0)
		*i = *i + open_file_and_save_fd(uni, (*last)->content, *i, type);
	else if (type == 0 && ((t_cmmnds *)(*last)->content)->broken == 0)
	{
		if (handle_wildcard(uni->commands[*i], (t_cmmnds *)(*last)->content))
			cmmnd_to_struct(((t_cmmnds *)(*last)->content), uni->commands[*i]);
		(*i)++;
	}
	else if (type == PIPE || type == AND || type == OR)
	{
		if (*last && (*last)->content)
			((t_cmmnds *)(*last)->content)->type = type;
		*last = next_cmmnd_struct(uni, *last, (*i)++);
	}
	else
		(*i)++;
}

void	cmd_array_to_struct(t_uni *uni)
{
	int		i;
	int		type;
	t_list	*last;

	i = 0;
	type = 0;
	uni->cmd_lst = ft_lstnew(create_cmmnd_struct(uni));
	last = uni->cmd_lst;
	while (uni->commands[i] && uni->stop == 0
		&& last != NULL && last->content != NULL)
	{
		type = get_type(uni, i);
		handle_command_type(uni, &last, &i, type);
	}
	if (i != 0 && i == array_length(uni->commands) && (uni->commands[i - 1]
			&& !ft_strcmp(uni->commands[i - 1], "|")))
		syntax_error(uni, "|");
}
