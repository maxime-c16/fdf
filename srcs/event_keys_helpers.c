/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_keys_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:30:14 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/23 13:13:15 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	handle_arrow_keys(int keycode)
{
	if (keycode == LEFT_ARROW)
		_fdf()->camera.x_offset -= 10;
	else if (keycode == RIGHT_ARROW)
		_fdf()->camera.x_offset += 10;
	else if (keycode == DOWN_ARROW)
		_fdf()->camera.y_offset -= 10;
	else if (keycode == UP_ARROW)
		_fdf()->camera.y_offset += 10;
	else if (keycode == M_KEY)
	{
		if (_fdf()->snap == 0)
			_fdf()->snap = 1;
		else
			_fdf()->snap = 0;
	}
}

void	handle_rotation_keys(int keycode, t_fdf *fdf)
{
	if (keycode == ADD_X_ROTATE)
		fdf->camera.rotation_x += 0.1;
	else if (keycode == SUB_X_ROTATE)
		fdf->camera.rotation_x -= 0.1;
	else if (keycode == ADD_Y_ROTATE)
		fdf->camera.rotation_y += 0.1;
	else if (keycode == SUB_Y_ROTATE)
		fdf->camera.rotation_y -= 0.1;
	else if (keycode == ADD_Z_ROTATE)
		fdf->camera.rotation_z += 0.1;
	else if (keycode == SUB_Z_ROTATE)
		fdf->camera.rotation_z -= 0.1;
	else if (keycode == ADD_SCALE)
	{
		fdf->scale += 0.25;
		fdf->camera.height_factor = ((double)fdf->camera.zoom
				/ max(1, fdf->max_altitude - fdf->min_altitude)) * fdf->scale;
	}
	else if (keycode == SUB_SCALE)
	{
		fdf->scale -= 0.25;
		fdf->camera.height_factor = ((double)fdf->camera.zoom
				/ max(1, fdf->max_altitude - fdf->min_altitude)) * fdf->scale;
	}
}

void	handle_zoom_key(int keycode)
{
	if (keycode == ADD_ZOOM)
		_fdf()->camera.zoom += 1;
	else if (keycode == SUB_ZOOM)
		_fdf()->camera.zoom -= 1;
}

void	handle_proj_key(int keycode)
{
	if (keycode == CH_PROJ)
	{
		_fdf()->proj_style++;
		if (_fdf()->proj_style >= 6)
			_fdf()->proj_style = 0;
	}
}

void	handle_reset_key(int keycode)
{
	if (keycode == RESET)
	{
		_fdf()->camera.zoom = (WIDTH + HEIGHT) / 40;
		_fdf()->camera.x_offset = WIDTH / 2;
		_fdf()->camera.y_offset = HEIGHT / 2;
		_fdf()->camera.rotation_x = 0.0;
		_fdf()->camera.rotation_y = 0.0;
		_fdf()->camera.rotation_z = 0.0;
		_fdf()->proj_style = PROJ_ISOMETRIC;
	}
}
