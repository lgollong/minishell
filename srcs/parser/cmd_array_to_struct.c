/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_array_to_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elgollong <elgollong@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 14:15:52 by rwegat            #+#    #+#             */
/*   Updated: 2024/12/22 16:20:29 by elgollong        ###   ########.fr       */
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
// bonus:
// type == 6 ?		&& AND
// type == 7 ?		|| OR
// type == 8 ?		( parentheses open
// type == 9 ?		) parentheses close
int	get_type(t_uni *uni, int i)
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
	if (type == 0 && ft_strcmp(uni->commands[i], "|") == 0)
		type = PIPE;

//bonus
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
	if (((t_cmmnds *)tmp->content)->outf == 1 && ((t_cmmnds *)tmp->content)->type != AND && ((t_cmmnds *)tmp->content)->type != OR)
		((t_cmmnds *)tmp->content)->outf = tube[1];
	else
		close(tube[1]);
	if (((t_cmmnds *)tmp->content)->inf == 0 && ((t_cmmnds *)tmp->content)->type != AND && ((t_cmmnds *)tmp->content)->type != OR)
		((t_cmmnds *)last->content)->inf = tube[0];
	else
		close(tube[0]);
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
	type = 0;
	uni->cmd_lst = ft_lstnew(create_cmmnd_struct(uni));
	last = uni->cmd_lst;
	while (uni->commands[i] && uni->stop == 0
		&& last != NULL && last->content != NULL)
	{
		type = get_type(uni, i);
		if (type == PAR_OPEN)
			uni->scope_p++;
		else if (type == PAR_CLOSE)
			uni->scope_p--;
		if (type > 0 && type < 5 && ((t_cmmnds *)last->content)->broken == 0)
			i = i + open_file_and_save_fd(uni, last->content, i, type);
		else if (type == 0 && ((t_cmmnds *)last->content)->broken == 0){
			if (handle_wildcard(uni->commands[i], (t_cmmnds *)last->content))
				cmmnd_to_struct(((t_cmmnds *)last->content), uni->commands[i]);
			i++;
		}
		else if (type == PIPE || type == AND || type == OR){
			if (last && last->content)
				((t_cmmnds *)last->content)->type = type;
			last = next_cmmnd_struct(uni, last, i++);
		}
		else
			i++;
	}
    // Debugging print to display the command structure
    printf("\033[31mCommand structure:\033[0m\n");
    t_list *current = uni->cmd_lst;
    int cmd_index = 0;
    while (current)
    {
        t_cmmnds *cmd = (t_cmmnds *)current->content;
        printf("\033[31mCommand %d: \033[0m", cmd_index++);
        for (int j = 0; cmd->cmd_array && cmd->cmd_array[j]; j++)
        {
            printf("\033[31m%s \033[0m", cmd->cmd_array[j]);
        }
        printf("\n");
        current = current->next;
    }
	printf("\n");
	if (i != 0 && i == array_length(uni->commands) && (uni->commands[i - 1]
			&& !ft_strcmp(uni->commands[i - 1], "|")))
		syntax_error(uni, "|");
}
