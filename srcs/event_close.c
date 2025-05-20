/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_close.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:29:55 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/20 14:29:56 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

int	on_close(void)
{
	ft_free_int_tab(_fdf()->map, _fdf()->height);
	mlx_destroy_image(_fdf()->mlx, _fdf()->img);
	mlx_destroy_window(_fdf()->mlx, _fdf()->win);
	mlx_destroy_display(_fdf()->mlx);
	free(_fdf()->mlx);
	exit(0);
}
