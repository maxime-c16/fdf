/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mcauchy <mcauchy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/14 18:34:29 by maximecauch       #+#    #+#             */
/*   Updated: 2025/02/10 12:57:20 by mcauchy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	copy_len;
	char	*str;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		copy_len = 0;
	else
	{
		if (s_len - start < len)
			copy_len = s_len - start;
		else
			copy_len = len;
	}
	str = (char *)malloc(sizeof(char) * (copy_len + 1));
	if (!str)
		return (NULL);
	if (copy_len > 0)
		ft_memcpy(str, s + start, copy_len);
	str[copy_len] = '\0';
	return (str);
}
