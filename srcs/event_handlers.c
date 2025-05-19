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

static void	handle_arrow_keys(int keycode)
{
	if (keycode == LEFT_ARROW)
		_fdf()->camera.x_offset -= 10;
	else if (keycode == RIGHT_ARROW)
		_fdf()->camera.x_offset += 10;
	else if (keycode == DOWN_ARROW)
		_fdf()->camera.y_offset -= 10;
	else if (keycode == UP_ARROW)
		_fdf()->camera.y_offset += 10;
}

static void	handle_rotation_keys(int keycode)
{
	if (keycode == ADD_X_ROTATE)
		_fdf()->camera.rotation_x += 0.1;
	else if (keycode == SUB_X_ROTATE)
		_fdf()->camera.rotation_x -= 0.1;
	else if (keycode == ADD_Y_ROTATE)
		_fdf()->camera.rotation_y += 0.1;
	else if (keycode == SUB_Y_ROTATE)
		_fdf()->camera.rotation_y -= 0.1;
	else if (keycode == ADD_Z_ROTATE)
		_fdf()->camera.rotation_z += 0.1;
	else if (keycode == SUB_Z_ROTATE)
		_fdf()->camera.rotation_z -= 0.1;
}

static void	handle_zoom_key(int keycode)
{
	if (keycode == ADD_ZOOM)
		_fdf()->camera.zoom += 1;
	else if (keycode == SUB_ZOOM)
		_fdf()->camera.zoom -= 1;
}

static void	handle_proj_key(int keycode)
{
	if (keycode == CH_PROJ)
	{
		_fdf()->proj_style++;
		if (_fdf()->proj_style >= 6)
			_fdf()->proj_style = 0;
	}
}

static void	handle_reset_key(int keycode)
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

int	key_hook(int keycode)
{
	if (keycode == ESC)
		on_close();
	handle_arrow_keys(keycode);
	handle_rotation_keys(keycode);
	handle_zoom_key(keycode);
	handle_proj_key(keycode);
	handle_reset_key(keycode);
	mlx_clear_window(_fdf()->mlx, _fdf()->win);
	draw_map();
	return (0);
}
