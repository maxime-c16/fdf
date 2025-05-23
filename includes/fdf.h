/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:04:24 by macauchy          #+#    #+#             */
/*   Updated: 2025/05/23 13:12:59 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H

# define FDF_H

# include "../mlx/mlx.h"
# include "../libft/libft.h"
# include <math.h>
# include <limits.h>

# define WIDTH 1120
# define HEIGHT 780

# define SEG 36

# define SENS_MOUSE 0.005

# ifndef M_PI
#  define M_PI 3.141592653
# endif

# ifdef __linux__
#  define ESC 65307
#  define ADD_X_ROTATE 119
#  define ADD_Y_ROTATE 97
#  define SUB_X_ROTATE 115
#  define SUB_Y_ROTATE 100
#  define SUB_Z_ROTATE 101
#  define ADD_Z_ROTATE 113
#  define ADD_SCALE 61
#  define SUB_SCALE 45
#  define LEFT 65361
#  define RIGHT 65363
#  define M_KEY 109

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

# define ADD_ZOOM 65451
# define SUB_ZOOM 65453
# define CH_PROJ 112
# define ESC 65307
# define RESET 114

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

# define MAX_SCALE 10.0

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
	void		*mlx;
	void		*win;
	void		*img;
	char		*img_data;
	int			bpp;
	int			size_line;
	int			endian;
	int			width;
	int			height;
	int			**map;
	int			fd;
	int			frame_count;
	int			flag;
	int			color;
	double		center_x;
	double		center_y;
	double		scale;
	int			snap;
	int			min_altitude;
	int			max_altitude;
	int			proj_style;
	int			mouse_pressed;
	int			mouse_x;
	int			mouse_y;
	t_camera	camera;
	t_gyro		gyro;
}				t_fdf;

typedef struct s_point
{
	int		x;
	int		y;
	int		z;
	int		color_altitude;
}			t_point;

typedef struct s_line
{
	t_point	a;
	t_point	b;
	t_point	start;
	int		sx;
	int		sy;
	int		err;
	double	total_dist;
}			t_line;

typedef struct s_drawmap_ctx
{
	t_fdf	*fdf;
	int		i;
	int		j;
}				t_drawmap_ctx;

typedef struct s_bresenham
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
}				t_bresenham;

typedef struct s_line_step
{
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	double	total_dist;
	t_point	a;
	t_point	b;
	t_point	start;
}	t_line_step;

typedef struct s_line_params
{
	t_point	a;
	t_point	start;
	t_point	b;
	double	t_param;
	double	current_z;
	double	total_dist;
}			t_line_params;

typedef struct s_proj_vars
{
	double	x;
	double	y;
	double	z;
	double	scale;
	double	cz;
}	t_proj_vars;

typedef struct s_scale_ctx
{
	double	scale;
	double	x;
	double	y;
	double	z;
	double	cz;
}	t_scale_ctx;

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
void	ft_free_tab(char **tab);
void	ft_free_int_tab(int **tab, int height);
double	lerp(double a, double b, double t);
int		linear_altitude_color(int altitude, int min_alt, int max_alt);
void	set_pixel(char *data, int x, int y, int current_z);
int		max(int a, int b);
void	compute_height_factor(t_fdf *fdf);
void	draw_line(t_point a, t_point b, char *img_data);
char	*ft_dtoa(double n, int precision);
void	draw_map(void);
void	print_map(void);
int		on_close(void);
int		key_hook(int keycode);
void	draw_line_c(t_point a, t_point b, int color);
void	get_gyro_point(t_gyro *g, int *px, int *py);
void	draw_colored_line(int *curr, int *v, char axis);
void	draw_gyroscope_sphere(t_fdf *fdf);
void	count_lines(char *filename);
int		read_from_fd(int fd);
void	init_draw_map_ctx(t_drawmap_ctx *ctx, int *i_start, int *i_step);
void	draw_map_loop(t_drawmap_ctx *ctx, int i_start, int i_step);
void	draw_gyroscope_sphere(t_fdf *fdf);
void	draw_map_rows(t_drawmap_ctx *ctx);
void	handle_arrow_keys(int keycode);
void	handle_rotation_keys(int keycode, t_fdf *fdf);
void	handle_zoom_key(int keycode);
void	handle_proj_key(int keycode);
void	handle_reset_key(int keycode);
void	apply_y_rotation(double *x, double *z, double theta);
void	apply_x_rotation(double *y, double *z, double phi);
void	apply_z_rotation(double *x, double *y, double psi);
void	apply_proj(t_point *point, double x, double y, double z);
t_point	project_point(int i, int j);
t_point	project_point_scaled(int i, int j);
void	apply_isometric_projection(t_point *point, double x, \
		double y, double z);

#endif
