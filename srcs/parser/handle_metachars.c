/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_metachars.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 15:15:50 by tilman            #+#    #+#             */
/*   Updated: 2022/12/16 16:06:38 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// like getenv() but returns allocated string
char	*env_to_str(t_uni *uni, char *str)
{
	char	*env_ptr;
	char	*new_str;

	if (!str)
		return (NULL);
	env_ptr = get_own_env(uni, str + 1);
	if (env_ptr)
	{
		new_str = ft_strdup(env_ptr);
		free(env_ptr);
		if (new_str)
		{
			free(str);
			str = new_str;
			return (str);
		}
		free(str);
		return (NULL);
	}
	else
	{
		free(str);
		return (NULL);
	}
}

// replace a part of a string with another string
char	*replace_part_of_str(char *str, int start, int end, char *rep_prt)
{
	char	*str_begin;
	char	*str_end;
	char	*buff;

	str_begin = ft_substr(str, 0, start);
	str_end = ft_substr(str, end, ft_strlen(str) - end);
	free(str);
	if (rep_prt)
	{
		buff = ft_strjoin(str_begin, rep_prt);
		str = ft_strjoin(buff, str_end);
		if (buff)
			free(buff);
	}
	else
		str = ft_strjoin(str_begin, str_end);
	if (str_begin)
		free(str_begin);
	if (str_end)
		free(str_end);
	return (str);
}

// check if char at *j's position is '$'.
// in case of '$' it replaces this part with its env. 
char	*replace_if_dollar(t_uni *uni, char *str, int *j)
{
	int		i;
	char	*buff;
	char	*num_str;

	i = 1;
	if (str[*j] == '$' && str[i + *j] == '?')
	{
		num_str = ft_itoa(g_exitcode);
		if (!num_str)
			return (NULL);
		str = replace_part_of_str(str, *j, *j + 2, num_str);
		*j = *j + ft_strlen(num_str) - 1;
		free(num_str);
	}
	else if (str[*j] == '$' && (ft_isalnum(str[*j + 1]) || str[*j + 1] == '_'))
	{
		while (str[i + *j] && (ft_isalnum(str[i + *j]) || str[i + *j] == '_'))
			i++;
		buff = ft_strldup(str + *j, i);
		buff = env_to_str(uni, buff);
		str = replace_part_of_str(str, *j, *j + i, buff);
		*j = *j + ft_strlen(buff) - 1;
		free(buff);
	}
	return (str);
}

// check if char at j's position has to be interpreted or deleted
char	*check_for_metachars(t_uni *uni, char *str, int *j, int *cnt)
{
	char	*home;

	if (cnt[1] % 2 == 0 && cnt[0] % 2 == 0
		&& (str[*j] == '~'))
	{
		home = get_own_env(uni, "HOME");
		str = replace_part_of_str(str, *j, *j + 1, home);
		*j = *j + (ft_strlen(home) - 1);
		free(home);
	}
	if (cnt[1] % 2 == 0 && cnt[0] % 2 == 0
		&& (str[*j] == '\\' || str[*j] == ';'))
	{
		str = replace_part_of_str(str, *j, *j + 1, NULL);
		(*j)--;
	}
	else if (cnt[1] % 2 == 0
		|| (cnt[1] % 2 != 0 && ft_strrchr(str + *j, '\'') == NULL))
		str = replace_if_dollar(uni, str, j);
	return (str);
}

void	handle_metachars(t_uni *uni, char **array)
{
	int	i;
	int	j;
	int	cnt[2];

	i = -1;
	while (array[++i])
	{
		j = -1;
		cnt[0] = 0;
		cnt[1] = 0;
		while (array[i][++j])
		{
			if (array[i][j] == '\'' && cnt[0] % 2 == 0)
				cnt[1]++;
			else if (array[i][j] == '\"' && cnt[1] % 2 == 0)
				cnt[0]++;
			else
				array[i] = check_for_metachars(uni, array[i], &j, cnt);
		}
		if (ft_strlen(array[i]) == 0)
			del_str_from_array(array, i--, 1);
	}
}
