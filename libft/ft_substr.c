/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 15:28:28 by tfriedri          #+#    #+#             */
/*   Updated: 2022/04/21 09:58:57 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*subs;
	size_t	reallen;

	i = 0;
	if (!s)
		return (NULL);
	if (ft_strlen(s + start) < len)
		reallen = ft_strlen(s + start);
	else
		reallen = len;
	subs = (char *)malloc((reallen + 1) * sizeof(char));
	if (!subs)
		return (NULL);
	while (i < reallen && ft_strlen(s) >= start)
	{
		subs[i] = s[start + i];
		i++;
	}
	subs[i] = '\0';
	return (subs);
}
