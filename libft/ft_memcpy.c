/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcauchy <mcauchy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 17:32:05 by maximecauch       #+#    #+#             */
/*   Updated: 2025/02/09 18:59:32 by mcauchy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*ptr;
	unsigned char	*temp;

	ptr = (unsigned char *)dst;
	temp = (unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	while (n)
	{
		*ptr++ = *temp++;
		n--;
	}
	return (dst);
}
