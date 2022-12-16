/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:31:54 by tfriedri          #+#    #+#             */
/*   Updated: 2022/04/21 09:58:05 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c)
{
	int	i;
	int	j;
	int	xbool;

	i = 0;
	j = 0;
	xbool = 0;
	while (s[i] != '\0')
	{
		if (s[i] == c && xbool == 1)
		{
			j++;
			xbool = 0;
		}
		else if (s[i] != c && s[i + 1] == '\0')
			j++;
		else if (s[i] != c)
			xbool = 1;
		i++;
	}
	return (j);
}

static const char	*ft_get_ptr_to_str(const char *s, char c, int strnum)
{
	int	i;
	int	j;
	int	xbool;

	i = 0;
	j = 0;
	xbool = 1;
	while (j < strnum)
	{
		if (s[i] == c)
		{
			i++;
			xbool = 1;
		}
		else if (s[i] != c && xbool == 1)
		{
			j++;
			xbool = 0;
		}
		else
			i++;
	}
	return (s + i);
}

static void	ft_cpy_str_to_arr(char *dst, const char *src, char c)
{
	int	i;

	i = 0;
	while (src[i] != c && src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

static size_t	ft_strpartlen(const char *str, const char c)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0' && str[i] != c)
		i++;
	return (i);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**retarray;

	if (!s)
		return (NULL);
	retarray = (char **)ft_calloc(sizeof(char *), (ft_count_words(s, c) + 1));
	if (!retarray)
		return (NULL);
	i = 0;
	while (i < ft_count_words(s, c))
	{
		retarray[i] = (char *)ft_calloc(sizeof(char), (ft_strpartlen(
						ft_get_ptr_to_str(s, c, i + 1), c) + 1));
		if (!retarray[i])
		{
			while (--i >= 0)
				free(retarray[i]);
			free(retarray);
			return (NULL);
		}
		ft_cpy_str_to_arr(retarray[i], ft_get_ptr_to_str(s, c, i + 1), c);
		i++;
	}
	retarray[i] = (NULL);
	return (retarray);
}
