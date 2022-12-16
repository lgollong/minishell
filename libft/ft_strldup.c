/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strldup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tilman <tfriedri@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 10:28:13 by tfriedri          #+#    #+#             */
/*   Updated: 2022/10/25 15:50:01 by tilman           ###   ########.fr       */
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
