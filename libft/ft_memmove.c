/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 20:46:24 by tfriedri          #+#    #+#             */
/*   Updated: 2022/04/21 09:57:36 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t		i;
	char		*d;
	const char	*s;

	i = 0;
	d = dst;
	s = src;
	if (!dst && !src)
		return (dst);
	if (src < dst)
	{
		while (len-- > 0)
			d[len] = s[len];
	}
	else
	{
		while (i++ < len)
			d[i - 1] = s[i - 1];
	}
	return (dst);
}
