/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 08:17:17 by tfriedri          #+#    #+#             */
/*   Updated: 2022/04/21 09:58:30 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	cpysize;

	if (dstsize > ft_strlen(src))
		cpysize = ft_strlen(src);
	else
		cpysize = dstsize - 1;
	if (dstsize != 0)
	{
		ft_memcpy(dst, src, cpysize);
		*(dst + cpysize) = '\0';
	}
	return (ft_strlen(src));
}
