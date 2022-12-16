/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 17:00:11 by tfriedri          #+#    #+#             */
/*   Updated: 2022/04/21 09:58:53 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_contains_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static char	*ft_malloc_empty_ptr_str(void)
{
	char	*str;

	str = (char *)malloc(sizeof(char));
	if (!str)
		return (NULL);
	str[0] = '\0';
	return (str);
}

static char	*ft_malloc_ptr_str(long long begin, long long end, const char *str)
{
	char	*rstr;
	size_t	size;
	size_t	i;

	size = ((ft_strlen(str) - (begin + (ft_strlen(str) - end))) + 1);
	rstr = (char *)malloc(sizeof(char) * size);
	if (!rstr)
		return (NULL);
	i = 0;
	while (begin < end)
	{
		rstr[i] = str[begin];
		i++;
		begin++;
	}
	rstr[i] = '\0';
	return (rstr);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	long long	begin;
	long long	end;
	char		*rstr;

	if (!s1 || !set)
		return (NULL);
	begin = 0;
	end = ft_strlen(s1);
	while (ft_contains_char((char *)set, s1[begin]) == 1)
		begin++;
	while (ft_contains_char((char *)set, s1[end - 1]) == 1 && end > 1)
		end--;
	if (((long long)ft_strlen(s1) - (begin
				+ ((long long)ft_strlen(s1) - end))) <= 0)
		rstr = ft_malloc_empty_ptr_str();
	else
		rstr = ft_malloc_ptr_str(begin, end, s1);
	if (!rstr)
		return (NULL);
	return (rstr);
}
