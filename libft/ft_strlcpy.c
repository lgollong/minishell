/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 08:17:17 by lgollong          #+#    #+#             */
/*   Updated: 2024/12/10 15:30:50 by lgollong         ###   ########.fr       */
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
