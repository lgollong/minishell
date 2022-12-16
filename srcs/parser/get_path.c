/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 18:10:35 by tfriedri          #+#    #+#             */
/*   Updated: 2022/12/16 17:37:44 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**split_env_paths(char *envp[])
{
	char	**paths;
	int		i;

	i = -1;
	paths = NULL;
	if (!envp)
		return (NULL);
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			if (!paths)
				return (NULL);
		}
	}
	return (paths);
}

static char	*check_path(char *path, char *cmmnd)
{
	char	*s1;
	char	*s2;

	if (!path || !cmmnd)
		return (NULL);
	s1 = ft_strjoin(path, "/");
	s2 = ft_strjoin(s1, cmmnd);
	if (s1)
		free(s1);
	if (!s2)
		return (NULL);
	if (access(s2, X_OK) == 0)
		return (s2);
	free(s2);
	return (NULL);
}

char	*get_path(char *cmmnd, char *envp[])
{
	int		i;
	char	**paths;
	char	*cmmnd_path;

	if (!access(cmmnd, X_OK) && !access(cmmnd, F_OK))
		return (ft_strdup(cmmnd));
	if (!envp || (ft_strchr(cmmnd, '/') && access(cmmnd, F_OK) < 0))
		return (NULL);
	paths = split_env_paths(envp);
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		cmmnd_path = check_path(paths[i], cmmnd);
		if (cmmnd_path)
		{
			clean_array(&paths);
			return (cmmnd_path);
		}
	}
	clean_array(&paths);
	if (!access(cmmnd, F_OK))
		return (ft_strdup(cmmnd));
	return (NULL);
}
