/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_keys.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:05:51 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/23 13:15:05 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

int	key_hook(int keycode)
{
	if (keycode == ESC)
		on_close();
	handle_arrow_keys(keycode);
	handle_rotation_keys(keycode, _fdf());
	handle_zoom_key(keycode);
	handle_proj_key(keycode);
	handle_reset_key(keycode);
	mlx_clear_window(_fdf()->mlx, _fdf()->win);
	draw_map();
	return (0);
}
