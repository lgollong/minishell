/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elgollong <elgollong@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 17:47:55 by rwegat            #+#    #+#             */
/*   Updated: 2024/12/22 11:43:43 by elgollong        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	edit_printed_name(char *filename, char *wildcard)
{
	
	DIR				*dir;

	(void)wildcard;
	dir = opendir(filename);
	if (!dir)
		return ;
	else
		filename = ft_strjoin(filename, "/");
	closedir(dir);
}

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
	if (ft_strcmp(*wildcard, "*/") || \
	ft_strnstr(*wildcard, "\\*", strlen(*wildcard)))
		edit_printed_name(*filename, *wildcard);
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
		if (match_wildcard(entry->d_name, command) && entry->d_name[0] != '.')
		{
			cmmnd_to_struct(content, entry->d_name);
			match_found = 1;
		}
		entry = readdir(dir);
	}
	if (match_found == 0)
		cmmnd_to_struct(content, command);
	closedir(dir);
	return (0);
}
