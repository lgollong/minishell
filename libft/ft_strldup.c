/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strldup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgollong <lgollong@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 10:28:13 by lgollong          #+#    #+#             */
/*   Updated: 2024/12/10 15:30:50 by lgollong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// char	*ft_strdup(const char *s1)
// {
// 	int		slen;
// 	char	*rptr;

// 	slen = ft_strlen(s1);
// 	rptr = (char *)malloc((slen + 1) * sizeof(char));
// 	if (rptr != NULL)
// 		ft_strlcpy(rptr, s1, slen + 1);
// 	return (rptr);
// }

char	*ft_strldup(const char *s1, int len)
{
	int		slen;
	char	*rptr;

	slen = ft_strlen(s1);
	if (len < slen)
		slen = len;
	rptr = (char *)malloc((slen + 1) * sizeof(char));
	if (rptr != NULL)
		ft_strlcpy(rptr, s1, slen + 1);
	return (rptr);
}
