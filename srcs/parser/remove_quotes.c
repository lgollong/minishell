/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 09:08:27 by tilman            #+#    #+#             */
/*   Updated: 2022/12/06 17:03:38 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// removes char at i's position from str
void	rem_char_from_str(char *str, int i)
{
	while (str[++i])
		str[i - 1] = str[i];
	str[i - 1] = '\0';
}

// remove quote at j's position and also the next same quote
void	rem_quote_from_str(char *str, int *j)
{
	char	quote;

	quote = str[*j];
	rem_char_from_str(str, *j);
	while (str[*j])
	{
		if (str[*j] == quote)
		{
			rem_char_from_str(str, *j);
			(*j)--;
			break ;
		}
		(*j)++;
	}
}

// removes all quotes, that are NOT inside other closed quotes
void	remove_quotes_from_array(char **array)
{
	int	i;
	int	j;

	i = -1;
	while (array[++i])
	{
		j = -1;
		while (array[i][++j])
		{
			if (array[i][j] == '\'' || array[i][j] == '\"')
				rem_quote_from_str(array[i], &j);
		}
	}
}

// removes quotes from the cmd_array of a s_commands-struct
void	remove_quotes_from_structs(void *content)
{
	t_cmmnds	*cmmnd_struct;

	cmmnd_struct = (t_cmmnds *)content;
	if (cmmnd_struct->cmd_array)
		remove_quotes_from_array(cmmnd_struct->cmd_array);
}
