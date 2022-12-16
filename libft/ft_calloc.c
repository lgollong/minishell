/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfriedri <tfriedri@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 09:36:39 by tfriedri          #+#    #+#             */
/*   Updated: 2022/04/21 09:56:13 by tfriedri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*rtp;

	rtp = malloc(count * size);
	if (rtp != NULL)
		ft_bzero(rtp, count * size);
	return (rtp);
}
