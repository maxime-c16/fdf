/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:04:24 by macauchy          #+#    #+#             */
/*   Updated: 2025/04/02 13:59:42 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H

# define FDF_H

# include "../mlx/mlx.h"
# include "../ft_gl/include/libft_gl.h"
# include "../libft/libft.h"
# include <math.h>
# include <limits.h>
# include <portaudio.h>
# include <fftw3.h>
# include <string.h>
# include <strings.h>
# include <sys/time.h>

# define WIDTH 800
# define HEIGHT 600

# define AUDIO_BUFFER_SIZE 512

# define AUDIO_MAP_WIDTH  200
# define AUDIO_MAP_HEIGHT 50
# define MAX_HEIGHT_AUDIO 250
# define SAMPLE_RATE 44100.0

# define SPECTRO_FREQ_START 20
# define SPECTRO_FREQ_END 20000
# define SPECTRO_HEIGHT	5

# define FLUX_BUFFER_SIZE 512
# define BEAT_WINDOW_SEC 4
# define EFFECTIVE_FREQ_RANGE 5.0

# define SEG 36

# define SENS_MOUSE 0.005

# define LEFT_ARROW 123
# define RIGHT_ARROW 124
# define DOWN_ARROW 125
# define UP_ARROW 126
# define ADD_X_ROTATE 13
# define SUB_X_ROTATE 1
# define ADD_Y_ROTATE 0
# define SUB_Y_ROTATE 2
# define ADD_Z_ROTATE 14
# define SUB_Z_ROTATE 12
# define ADD_ZOOM 69
# define SUB_ZOOM 78
# define ADD_DEBUG 92
# define SUB_DEBUG 67
# define CH_PROJ 35
# define ESC 53
# define RESET 15

# define PROJ_ISOMETRIC 0
# define PROJ_PERSPECTIVE 1
# define PROJ_PARALLEL 2
# define PROJ_OBLIQUE 3
# define PROJ_CABINET 4
# define PROJ_CONIC 5

typedef struct s_bpm
{
	double		bpm;
	double		flux_prev[AUDIO_BUFFER_SIZE / 2 + 1];
	double		flux_history[FLUX_BUFFER_SIZE];
	int			index_history;
	double		last_beat_time;
	double		beat_intervals[FLUX_BUFFER_SIZE];
	int			beat_count;
	double		smooth_flux;
	double		last_update;
}				t_bpm;

typedef struct s_camera
{
	double		zoom;
	int		x_offset;
	int		y_offset;
	double	rotation_x;
	double	rotation_y;
	double	rotation_z;
	double	height_factor;
	double	save_height_factor;
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
}			t_gyro;

typedef struct s_audio
{
	PaStream		*stream;
	float			buffer[AUDIO_BUFFER_SIZE];
	float			volume;
	double			*fft_in;
	double			*fft_out;
	fftw_plan		fft_plan;
	double			*freq;
	int				channel_count;
	int				start_index;
	int				spectro_size;
}				t_audio;

typedef struct s_map
{
	int		**map;
	int		width;
	int		height;
}			t_map;

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
	double	debug;
	t_audio	audio;
	t_map		*audio_map;
	t_camera	camera;
	t_gyro	gyro;
	t_ft_gl	*gl;
	t_bpm		bpm_data;
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
char	*ft_dtoa(double n, int precision);
int		ft_clamp(int a, int min, int max);
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
void	start_audio_capture(void);
void	stop_audio_capture(void);

int		update_and_draw(void *param);
void	update_map_from_audio(t_fdf *fdf);

void	draw_line(t_point a, t_point b, char *img_data);
void	draw_music_map(void);

void	update_bpm(t_fdf *fdf, double current_time);

#endif
