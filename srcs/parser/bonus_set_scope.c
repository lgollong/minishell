/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_set_scope.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:02:07 by rwegat            #+#    #+#             */
/*   Updated: 2024/12/12 16:33:40 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//increse scope when '(' and decrease when ')'
int	set_scope(t_list	*cmd_lst, int type)
{
	int		current_scope;
	t_cmmnds	*current;

	current_scope = 0;
	current = (t_cmmnds *)cmd_lst->content;
	printf("current command: %s\n", current->cmd_array[0]);
	printf("Type: %d\n", type);
	if (type == PAR_OPEN)
		current_scope++;
	else if (type == PAR_CLOSE)
		current_scope--;
	current->scope = current_scope;
	printf("Scope: %d\n", current->scope);
	current = current->right;
}
