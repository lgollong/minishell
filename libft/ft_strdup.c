/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 10:28:13 by tfriedri          #+#    #+#             */
/*   Updated: 2022/04/21 09:58:12 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		slen;
	char	*rptr;

	slen = ft_strlen(s1);
	rptr = (char *)malloc((slen + 1) * sizeof(char));
	if (rptr != NULL)
		ft_strlcpy(rptr, s1, slen + 1);
	return (rptr);
}
