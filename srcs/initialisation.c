/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialisation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 14:01:05 by tfriedri          #+#    #+#             */
/*   Updated: 2022/12/15 11:56:29 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// like getenv but for the own envp
char	*get_own_env(t_uni *uni, char *name)
{
	int		i;
	char	*env_str;
	char	*str;
	int		len;

	i = -1;
	if (!name)
		return (NULL);
	str = ft_strjoin(name, "=");
	len = ft_strlen(str);
	while (str && uni->envp[++i])
	{
		if (!ft_strncmp(uni->envp[i], str, len))
		{
			env_str = ft_strdup(uni->envp[i] + len);
			return (free(str), env_str);
		}
	}
	if (str)
		free(str);
	return (NULL);
}

// copies the environment variables into minishell
char	**dup_envp(char **ep)
{
	int		i;
	int		len;
	char	**new_envp;

	if (!ep)
		return (NULL);
	len = array_length(ep);
	i = 0;
	new_envp = ft_calloc(len + 1, sizeof(char *));
	while (ep[i])
	{
		new_envp[i] = ft_strdup(ep[i]);
		if (!new_envp[i])
		{
			clean_array(&new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

// creates own envp
void	create_environment(t_uni *uni)
{
	uni->envp = ft_calloc(1, sizeof(char *));
	if (uni->envp)
	{
		uni->envp = add_str_to_array(uni->envp, "SHLVL=1");
		uni->envp = add_str_to_array(uni->envp, "_=");
		uni->envp = add_str_to_array(uni->envp, "PWD=");
		uni->envp = change_in_env("PWD=", getcwd(NULL, 0), uni->envp);
	}
}

// gets the parents envp or starts to create own
void	get_environment(t_uni *uni, char **envp)
{
	char		*shlvl_str;
	int			shlvl;

	if (envp[0])
	{
		uni->envp = dup_envp(envp);
		shlvl_str = get_own_env(uni, "SHLVL");
		if (!shlvl_str)
			uni->envp = change_in_env("SHLVL=", ft_itoa('1'), uni->envp);
		else
		{
			shlvl = ft_atoi(shlvl_str) + 1;
			free(shlvl_str);
			uni->envp = change_in_env("SHLVL=", ft_itoa(shlvl), uni->envp);
		}
	}
	else
		create_environment(uni);
}

// initialise the uni-struct
t_uni	*init_uni_struct(int argc, char *argv[], char **envp)
{
	t_uni		*uni;

	(void)argc;
	(void)argv;
	uni = ft_calloc(1, sizeof(t_uni));
	if (!uni)
		exit(1);
	uni->input_string = NULL;
	uni->commands = NULL;
	uni->envp = NULL;
	uni->pid = -1;
	uni->stop = 0;
	get_environment(uni, envp);
	uni->cmd_lst = NULL;
	return (uni);
}
