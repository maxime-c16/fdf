/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_map_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:45:33 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 12:45:39 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

char	*ft_dtoa(double n, int precision)
{
	char	*str;
	char	*tmp;
	int		int_part;
	int		dec_part;
	int		i;

	int_part = (int)n;
	dec_part = (int)((n - int_part) * pow(10, precision));
	str = ft_itoa(int_part);
	tmp = ft_strjoin(str, ".");
	free(str);
	str = ft_strjoin(tmp, ft_itoa(dec_part));
	free(tmp);
	i = ft_strlen(str) - 1;
	while (str[i] == '0')
	{
		str[i] = '\0';
		i--;
	}
	if (str[i] == '.')
		str[i] = '\0';
	return (str);
}
