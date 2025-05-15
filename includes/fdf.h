/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:04:24 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/15 15:06:50 by macauchy         ###   ########.fr       */
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

# define SEG 36

# define SENS_MOUSE 0.005

# ifndef M_PI
#  define M_PI 3.141592653
# endif

# ifdef __linux__
#  define ESC 65307
#  define W 119
#  define A 97
#  define S 115
#  define D 100
#  define E 101
#  define R 114
#  define LEFT 65361
#  define RIGHT 65363

# elif __APPLE__
#  define KEY_ESC 53
#  define W 13
#  define E 14
#  define R 15
#  define A 0
#  define S 1
#  define D 2
#  define LEFT 123
#  define RIGHT 124

# endif

# define UP_ARROW 	65362
# define DOWN_ARROW 65364
# define LEFT_ARROW 65361
# define RIGHT_ARROW 65363

// # define LEFT_ARROW 123
// # define RIGHT_ARROW 124
// # define DOWN_ARROW 125
// # define UP_ARROW 126
# define ADD_X_ROTATE 13
# define SUB_X_ROTATE 1
# define ADD_Y_ROTATE 0
# define SUB_Y_ROTATE 2
# define ADD_Z_ROTATE 14
# define SUB_Z_ROTATE 12
# define ADD_ZOOM 69
# define SUB_ZOOM 78
# define CH_PROJ 35
# define ESC 65307
# define RESET 15

# define PROJ_ISOMETRIC 0
# define PROJ_PERSPECTIVE 1
# define PROJ_PARALLEL 2
# define PROJ_OBLIQUE 3
# define PROJ_CABINET 4
# define PROJ_CONIC 5

# define ON_KEYDOWN 2
# define ON_KEYUP 3
# define ON_MOUSEDOWN 4
# define ON_MOUSEUP 5
# define ON_MOUSEMOVE 6
# define ON_EXPOSE 12
# define ON_DESTROY 17

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

typedef struct s_gyro
{
	char	axis;
	double	angle;
	double	rad;
	double	*rx;
	double	*ry;
	double	*rz;
	int		cx;
	int		cy;
	double	height_factor;
}				t_gyro;

typedef struct s_fdf
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*img_data;
	int		bpp;
	int		size_line;
	int		endian;
	int		width;
	int		height;
	int		**map;
	int		fd;
	int		frame_count;
	double	center_x;
	double	center_y;
	int		min_altitude;
	int		max_altitude;
	int		proj_style;
	int		mouse_pressed;
	int		mouse_x;
	int		mouse_y;
	t_camera	camera;
	t_gyro	gyro;
}			t_fdf;

typedef struct s_point
{
	int		x;
	int		y;
	int		z;
	int		color_altitude;
}			t_point;

t_fdf	*_fdf(void);
int		max(int a, int b);
void	parsing(char *filename);
void	draw_map(void);
t_point	project_point(int i, int j);
t_point	project_point_scaled(int i, int j);
void	apply_proj(t_point *point, double x, double y, double z);
void	set_pixel(char *data, int x, int y, int current_z);
double	lerp(double a, double b, double t);
void	draw_gyroscope_sphere(t_fdf *fdf);
void	apply_y_rotation(double *x, double *z, double theta);
void	apply_x_rotation(double *y, double *z, double phi);
void	apply_z_rotation(double *x, double *y, double psi);
int		mouse_press(int button, int x, int y);
int		mouse_release(int x, int y);
int		mouse_move(int x, int y);

#endif
