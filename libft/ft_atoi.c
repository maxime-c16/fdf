/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 12:54:51 by maximecauch       #+#    #+#             */
/*   Updated: 2025/05/23 12:24:08 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	ft_atoi(const char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (*str && (*str == '\t' || *str == '\n' || *str == '\v' || *str == '\f'
			|| *str == '\r'
			|| *str == ' '))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (ft_isdigit(*str) && *str)
	{
		if (result > 2147483647 && sign > 0)
			return (FLAG_OVERFLOW);
		if (result >= 2147483648 && sign < 0)
			return (FLAG_UNDERFLOW);
		result = result * 10 + (*str - '0');
		str++;
	}
	return ((int)result * sign);
}
