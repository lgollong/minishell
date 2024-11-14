/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:47:55 by rwegat            #+#    #+#             */
/*   Updated: 2024/11/14 12:34:50 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// example a*middle_text*z*A*a
int matchWildcard(char *filename, char *command)
{
	char *star = NULL;
	char *str = NULL;

	while (*filename)
	{
		if (*command == '*')
		{
			star = command++;
			str = filename;
		}
		else if (*command == '\\' && *(command + 1) == '*')
		{
			command += 2;
			if (*filename != '*')
				return 0;
			filename++;
		}
		else if (*command == '\'' && *(command + 1) == '*' && *(command + 2) == '\'')
		{
			command += 3;
			if (*filename != '*')
				return 0;
			filename++;
		}
		else if (*command == *filename)
		{
			command++;
			filename++;
		}
		else if (star)
		{
			command = star + 1;
			filename = ++str;
		}
		else
			return 0;
	}

	while (*command == '*')
		command++;

	return (!*command);
}


// Test function to demonstrate usage
int	handle_wildcard(char *command, t_cmmnds *content)
{
	DIR *dir;
	struct dirent *entry;
	int match_found;

	match_found = 0;
	if (!ft_strchr(command, '*'))
		return (1);
	dir = opendir(".");
	if (!dir)
		return (1);
	while ((entry = readdir(dir)) != NULL)
	{
		if (matchWildcard(entry->d_name, command))
		{
			cmmnd_to_struct(content, entry->d_name);
			match_found = 1;
		}
	}
	if (match_found == 0)
		cmmnd_to_struct(content,command);
	closedir(dir);
	return (0);
}
