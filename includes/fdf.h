/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:04:24 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/20 15:38:16 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H

# define FDF_H

# include "../mlx/mlx.h"
# include "../libft/libft.h"
# include <math.h>
# include <limits.h>

# define WIDTH 800
# define HEIGHT 600

# define LEFT_ARROW 123
# define RIGHT_ARROW 124
# define DOWN_ARROW 125
# define UP_ARROW 126
# define ADD_X_ROTATE 13
# define SUB_X_ROTATE 1
# define ADD_Y_ROTATE 0
# define SUB_Y_ROTATE 2
# define ADD_Z_ROTATE 12
# define SUB_Z_ROTATE 14

typedef struct s_camera
{
	int		zoom;
	int		x_offset;
	int		y_offset;
	double	rotation_x;
	double	rotation_y;
	double	rotation_z;
	double	height_factor;
}			t_camera;

typedef struct s_fdf
{
	void	*mlx;
	void	*win;
	int		width;
	int		height;
	int		**map;
	int		fd;
	int		frame_count;
	double	center_x;
	double	center_y;
	t_camera	camera;
}			t_fdf;

typedef struct s_point
{
	int		x;
	int		y;
	int		z;
	int		color_altitude;
}			t_point;

t_fdf	*_fdf(void);
void	parsing(char *filename);
t_point	project_point(int i, int j);

#endif
