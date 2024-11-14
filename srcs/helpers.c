/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 01:41:07 by tfriedri          #+#    #+#             */
/*   Updated: 2024/11/14 12:08:33 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// frees a string-array and sets its pointer to NULL
void	clean_array(char ***array)
{
	int	i;

	i = -1;
	if (!*array)
		return ;
	while ((*array)[++i])
		free((*array)[i]);
	free(*array);
	*array = NULL;
}

// returns the length of a string-array
int	array_length(char	**array)
{
	int	i;

	i = 0;
	if (!array)
		return (0);
	while (array[i])
		i++;
	return (i);
}

// deletes a string from a string-array
void	del_str_from_array(char **array, int del_pos, int del_quant)
{
	int	i;
	int	del_pos_buff;

	i = 0;
	while (i++ < del_quant)
	{
		del_pos_buff = del_pos;
		free(array[del_pos_buff]);
		while (array[++del_pos_buff])
			array[del_pos_buff - 1] = array[del_pos_buff];
		array[del_pos_buff - 1] = NULL;
	}
}

// appends str at the end of the array and returns an pointer to the new array
char	**add_str_to_array(char **array, char *str)
{
	char	**new;
	int		len;

	len = array_length(array);
	new = ft_calloc(len + 2, sizeof(char *));
	if (!new)
		return (NULL);
	new[len] = ft_strdup(str);
	while (--len >= 0)
		new[len] = ft_strdup(array[len]);
	clean_array(&array);
	return (new);
}

// creates a new str with the extension, returns it and frees the old one
char	*str_extend(char **str, char *extension)
{
	char	*new_str;

	if (!str || !*str)
	{
		new_str = ft_strdup(extension);
		return (new_str);
	}
	if (!extension)
	{
		new_str = ft_strdup(*str);
		return (new_str);
	}
	new_str = ft_strjoin(*str, extension);
	free(*str);
	return (new_str);
}
