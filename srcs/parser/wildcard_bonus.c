/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:47:55 by rwegat            #+#    #+#             */
/*   Updated: 2024/11/14 15:28:52 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// special chars to interpret * as a literal char 
void	handle_special_chars(char **wildcard, char **filename, \
char **star, char **str)
{
	if (**wildcard == '*')
	{
		*star = *wildcard;
		(*wildcard)++;
		*str = *filename;
		return ;
	}
	else if (**wildcard == '\\' && *(*wildcard + 1) == '*')
	{
		*wildcard += 2;
		if (**filename != '*')
			return ;
		(*filename)++;
	}
	else if (**wildcard == '\'' && *(*wildcard + 1) == '*' \
	&& *(*wildcard + 2) == '\'')
	{
		*wildcard += 3;
		if (**filename != '*')
			return ;
		(*filename)++;
	}
}

// compares the filename with the wildcard
int	match_wildcard(char *filename, char *wildcard)
{
	char	*star;
	char	*str;

	str = NULL;
	star = NULL;
	while (*filename)
	{
		if (*wildcard == '*' || *wildcard == '\'' || *wildcard == '\\')
			handle_special_chars(&wildcard, &filename, &star, &str);
		else if (*wildcard == *filename)
		{
			wildcard++;
			filename++;
		}
		else if (star)
		{
			wildcard = star + 1;
			filename = ++str;
		}
		else
			return (0);
	}
	while (*wildcard == '*')
		wildcard++;
	return (!*wildcard);
}

int	handle_wildcard(char *command, t_cmmnds *content)
{
	DIR				*dir;
	struct dirent	*entry;
	int				match_found;

	match_found = 0;
	if (!ft_strchr(command, '*'))
		return (1);
	dir = opendir(".");
	if (!dir)
		return (1);
	entry = readdir(dir);
	while (entry != NULL)
	{
		entry = readdir(dir);
		if (match_wildcard(entry->d_name, command))
		{
			cmmnd_to_struct(content, entry->d_name);
			match_found = 1;
		}
	}
	if (match_found == 0)
		cmmnd_to_struct(content, command);
	closedir(dir);
	return (0);
}
