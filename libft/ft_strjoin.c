/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 16:08:41 by tfriedri          #+#    #+#             */
/*   Updated: 2022/04/21 09:58:20 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		l1;
	int		l2;
	char	*rstr;

	if (!s1 || !s2)
		return (NULL);
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	rstr = (char *)malloc(sizeof(char) * (l1 + l2 + 1));
	if (!rstr)
		return (NULL);
	ft_memcpy(rstr, s1, l1);
	ft_memcpy(rstr + l1, s2, l2);
	rstr[l1 + l2] = '\0';
	return (rstr);
}
