/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rwegat <rwegat@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 14:15:23 by rwegat            #+#    #+#             */
/*   Updated: 2025/01/16 16:55:45 by rwegat           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check if it is allowed to cut the string
int	cut_allow_checker(char mark, int reset)
{
	static int	static_qm;
	static int	static_sq;

	if (reset == 1)
	{
		static_qm = 0;
		static_sq = 0;
		return (0);
	}
	if (mark == '\"' && static_sq % 2 == 0)
		static_qm++;
	else if (mark == '\'' && static_qm % 2 == 0)
		static_sq++;
	if (static_qm % 2 != 0 || static_sq % 2 != 0)
		return (1);
	else
		return (0);
}

unsigned int	ft_skip_word(char *str, unsigned int i)
{
	if ((str[i] == '|' && str[i + 1] == '|') || \
	(str[i] == '&' && str[i + 1] == '&'))
		i += 2;
	else
		i++;
	if (ft_strchr("<|>&()", str[i - 1]) == NULL)
	{
		while (ft_strchr("<|>&() ", str[i]) == NULL && \
		!((str[i] == '|' && str[i + 1] == '|') || \
		(str[i] == '&' && str[i + 1] == '&')))
			i++;
	}
	return (i);
}

// count the words/arguments
unsigned int	ft_word_count(char *str)
{
	unsigned int	counter;
	unsigned int	i;

	counter = 0;
	i = 0;
	cut_allow_checker('x', 1);
	while (str[i] == ' ')
		i++;
	while (str[i])
	{
		if (cut_allow_checker(str[i], 0) == 0 && str[i] != ' ')
		{
			counter++;
			i = ft_skip_word(str, i);
		}
		else
			i++;
		while (str[i] == ' ')
			i++;
	}
	return (counter);
}

// get string-length for the argument
unsigned int	ft_letter_count(const char *str, unsigned int i)
{
	unsigned int	k;

	k = 0;
	cut_allow_checker('x', 1);
	if ((str[i] == '|' && str[i + 1] == '|') || \
	(str[i] == '&' && str[i + 1] == '&'))
		return (2);
	if (ft_strchr("<|>&()", str[i]) != NULL)
		return (1);
	while (str[i] != '\0')
	{
		if (cut_allow_checker(str[i], 0) == 0
			&& ft_strchr("<|>&() ", str[i]) != NULL)
			break ;
		i++;
		k++;
	}
	return (k);
}

// splits the input string into an array
char	**split_input(char *str)
{
	unsigned int		i;
	unsigned int		j;
	unsigned int		str_len;
	unsigned int		wrd_cnt;
	char				**cmd_array;

	i = 0;
	j = 0;
	wrd_cnt = ft_word_count(str);
	cmd_array = (char **)ft_calloc(sizeof(char *), (wrd_cnt + 1));
	if (!cmd_array)
		return (NULL);
	while (str[i] == ' ')
		i++;
	while (str[i])
	{
		str_len = ft_letter_count(str, i);
		if (cut_allow_checker('x', 0) != 1)
			cmd_array[j++] = ft_substr(str, i, str_len);
		i = i + str_len;
		while (str[i] == ' ')
			i++;
	}
	cmd_array[wrd_cnt] = NULL;
	return (cmd_array);
}
