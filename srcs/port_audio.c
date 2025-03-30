/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   port_audio.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:22:40 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/25 14:42:52 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static double	ftmax(double a, double b)
{
	return (a > b ? a : b);
}

static double	ftmin(double a, double b)
{
	return (a < b ? a : b);
}

static int	audio_callback(const void *input, void *output,
		unsigned long frames, const PaStreamCallbackTimeInfo *time_info,
		PaStreamCallbackFlags status_flags, void *user_data)
{
	t_audio			*audio;
	const float		*in;
	unsigned long	i;
	float			vol_l;
	float			vol_r;
	int				bar_size;
	double			mag;

	(void)output;
	(void)time_info;
	(void)status_flags;
	audio = (t_audio *)user_data;
	in = (const float *)input;
	i = 0;
	vol_l = 0;
	vol_r = 0;
	mag = 0;
	bar_size = 100;
	while (i < frames)
	{
		audio->buffer[i] = in[i];
		if (i % audio->channel_count == 0)
			audio->fft_in[i / audio->channel_count] = in[i];
		i++;
	}
	for (int i = 0; i < AUDIO_BUFFER_SIZE * 2; i+=2)
	{
		vol_l = ftmax(vol_l, fabs(in[i]));
		vol_r = ftmax(vol_r, fabs(in[i + 1]));
	}
	audio->volume = (vol_l + vol_r) / 2;
	fftw_execute(audio->fft_plan);
	printf("\r");
	for (int i = 0; i < bar_size; i++)
	{
		double	ratio = pow(i / (double)bar_size, 4);
		double	freq = audio->fft_out[(int)(audio->start_index + ratio * audio->spectro_size)];

		if (freq < 0.125) {
			printf("▁");
		} else if (freq < 0.25) {
			printf("▂");
		} else if (freq < 0.375) {
			printf("▃");
		} else if (freq < 0.5) {
			printf("▄");
		} else if (freq < 0.625) {
			printf("▅");
		} else if (freq < 0.75) {
			printf("▆");
		} else if (freq < 0.875) {
			printf("▇");
		} else {
			printf("█");
		}
	}
	fflush(stdout);
	return (paContinue);
}

void	start_audio_capture(void)
{
	PaStreamParameters		parameters;
	PaError					err;
	t_fdf					*fdf;
	int						selectedDevice = -1;
	int						numDevices;
	const PaDeviceInfo		*deviceInfo;
	int						i;

	fdf = _fdf();
	fdf->audio.stream = NULL;
	err = Pa_Initialize();
	fdf->audio.fft_in = (double *)malloc(sizeof(double) * AUDIO_BUFFER_SIZE);
	fdf->audio.fft_out = (double *)malloc(sizeof(double) * AUDIO_BUFFER_SIZE);
	if (!fdf->audio.fft_in || !fdf->audio.fft_out)
	{
		dprintf(2, "fftwf_malloc() failed\n");
		exit(1);
	}
	fdf->audio.fft_plan = fftw_plan_r2r_1d(AUDIO_BUFFER_SIZE, fdf->audio.fft_in,
			fdf->audio.fft_out, FFTW_R2HC, FFTW_ESTIMATE);
	if (!fdf->audio.fft_plan)
	{
		dprintf(2, "fftwf_plan_dft_r2c_1d() failed\n");
		exit(1);
	}
	double	scale = AUDIO_BUFFER_SIZE / SAMPLE_RATE;
	fdf->audio.start_index = ceil(scale * SPECTRO_FREQ_START);
	fdf->audio.spectro_size = ftmin(ceil(scale * SPECTRO_FREQ_END),
		AUDIO_BUFFER_SIZE / 2.0) - fdf->audio.start_index;
	if (err != paNoError)
	{
		dprintf(2, "PortAudio error: %s\n", Pa_GetErrorText(err));
		exit(1);
	}
	/* Enumerate devices to find our virtual audio device */
	numDevices = Pa_GetDeviceCount();
	i = 0;
	while (i < numDevices)
	{
		deviceInfo = Pa_GetDeviceInfo(i);
		dprintf(2, "Device %d: %s\n", i, deviceInfo->name);
		if (deviceInfo && (strstr(deviceInfo->name, "Multi-Output") ||
			strstr(deviceInfo->name, "BlackHole")))
		{
			selectedDevice = i;
			printf("	device index: %d\n", selectedDevice);
			printf("	sample rate: %f\n", deviceInfo->defaultSampleRate);
			printf("	input channels: %d\n", deviceInfo->maxInputChannels);
			printf("	output channels: %d\n", deviceInfo->maxOutputChannels);
			printf("	input latency: %f\n", deviceInfo->defaultLowInputLatency);
		}
		i++;
	}
	if (selectedDevice < 0)
	{
		ft_putstr_fd("Error: Virtual input device not found\n", 2);
		exit(1);
	}
	parameters.device = 0;
	parameters.channelCount = 2;
	fdf->audio.channel_count = parameters.channelCount;
	parameters.sampleFormat = paFloat32;
	parameters.suggestedLatency = Pa_GetDeviceInfo(parameters.device)->defaultLowInputLatency;
	parameters.hostApiSpecificStreamInfo = NULL;
	err = Pa_OpenStream(&fdf->audio.stream, &parameters, NULL,
			SAMPLE_RATE, AUDIO_BUFFER_SIZE, paClipOff, audio_callback, &fdf->audio);
	if (err != paNoError)
	{
		dprintf(2, "PortAudio error: %s\n", Pa_GetErrorText(err));
		exit(1);
	}
	err = Pa_StartStream(fdf->audio.stream);
	if (err != paNoError)
	{
		dprintf(2, "PortAudio error: %s\n", Pa_GetErrorText(err));
		exit(1);
	}
	printf("Audio capture started\n");
}

void	stop_audio_capture(void)
{
	t_fdf	*fdf;
	PaError	err;

	fdf = _fdf();
	if (fdf->audio.stream)
	{
		err = Pa_StopStream(fdf->audio.stream);
		if (err != paNoError)
			dprintf(2, "PortAudio error: %s\n", Pa_GetErrorText(err));
		err = Pa_CloseStream(fdf->audio.stream);
		if (err != paNoError)
			dprintf(2, "PortAudio error: %s\n", Pa_GetErrorText(err));
		Pa_Terminate();
		fdf->audio.stream = NULL;
	}
	printf("Audio capture stopped\n");
}

static int	**allocate_audio_map(void)
{
	int		**map;
	int		i;

	i = 0;
	map = (int **)malloc(sizeof(int *) * AUDIO_MAP_WIDTH);
	if (!map)
		return (NULL);
	while (i < AUDIO_MAP_WIDTH)
	{
		map[i] = (int *)malloc(sizeof(int) * AUDIO_MAP_HEIGHT);
		if (!map[i])
			return (NULL);
		i++;
	}
	return (map);
}

void	update_map_from_audio(t_fdf *fdf)
{
	int		i;
	int		j;
	int		sample_index;
	float	*buffer;
	int		**map;
	int		n;

	if (fdf->audio_map == NULL)
	{
		fdf->audio_map = (t_map *)malloc(sizeof(t_map));
		if (!fdf->audio_map)
			return ;
		fdf->audio_map->map = allocate_audio_map();
		if (!fdf->audio_map->map)
			return ;
		fdf->audio_map->width = AUDIO_MAP_WIDTH;
		fdf->audio_map->height = AUDIO_MAP_HEIGHT;
	}
	buffer = (float *)fdf->audio.buffer;
	n = AUDIO_BUFFER_SIZE;
	j = 0;
	while (j < AUDIO_MAP_WIDTH)
	{
		sample_index = (j * n) / AUDIO_MAP_WIDTH;
		float sample = (((buffer[sample_index] + 1.0) / 2.0));
		fdf->audio_map->map[0][j] = (int)(pow(sample, 1.1) * MAX_HEIGHT_AUDIO);
		j++;
	}
	i = 1;
	while (i < AUDIO_MAP_HEIGHT)
	{
		j = 0;
		while (j < AUDIO_MAP_WIDTH)
		{
			fdf->audio_map->map[i][j] = fdf->audio_map->map[0][j];
			j++;
		}
		i++;
	}
	fdf->width = AUDIO_MAP_WIDTH;
	fdf->height = AUDIO_MAP_HEIGHT;
}

int	update_and_draw(void *param)
{
	t_fdf	*fdf;

	fdf = (t_fdf *)param;
	// update_map_from_audio(fdf);
	mlx_clear_window(fdf->mlx, fdf->win);
	draw_music_map();
	// draw_map();
	return (0);
}

double	ft_dclamp(double x, double min, double max)
{
	if (x < min)
		return (min);
	if (x > max)
		return (max);
	return (x);
}

t_point	project_spectro_pts(int i, int j)
{
	t_fdf	*fdf;
	t_point	point;
	double	x;
	double	y;
	double	z;
	int		freq_bin;
	double	freq_scale;
	double	center_x;
	double	center_y;
	double	ratio;

	fdf = _fdf();
	center_x = ((AUDIO_BUFFER_SIZE / 2.0) * fdf->camera.zoom) / 2.0;
	center_y = (SPECTRO_HEIGHT * fdf->camera.zoom) / 2.0;
	x = j * fdf->camera.zoom - center_x;
	y = i * fdf->camera.zoom - center_y;
	freq_bin = j;
	freq_scale = fabs(fdf->audio.fft_out[j]) * 10.0;
	ratio = (double)freq_bin / (double)(AUDIO_BUFFER_SIZE / 2.0);
	freq_scale = pow(freq_scale, (double)(0.6 + (1.2 - 0.6) * ratio));
	z = (fdf->audio.fft_out[freq_bin] > 0.0) ? freq_scale : -freq_scale;
	z *= (fdf->audio.volume * 5.0);
	// z = ft_dclamp(z, -MAX_HEIGHT_AUDIO, MAX_HEIGHT_AUDIO);
	apply_y_rotation(&x, &z, fdf->camera.rotation_y);
	apply_x_rotation(&y, &z, fdf->camera.rotation_x);
	apply_z_rotation(&x, &y, fdf->camera.rotation_z);
	apply_proj(&point, x, y, z);
	point.x += fdf->camera.x_offset;
	point.y += fdf->camera.y_offset;
	point.z = z;
	return (point);
}

void	draw_music_map(void)
{
	t_fdf	*fdf;
	int		i;
	int		j;
	t_point	a;
	t_point	b;

	fdf = _fdf();
	i = 0;
	fdf->img_data = mlx_get_data_addr(fdf->img, &fdf->bpp, &fdf->size_line, &fdf->endian);
	ft_bzero(fdf->img_data, fdf->size_line * HEIGHT);
	if (fdf->camera.rotation_x > 0.0)
		i = SPECTRO_HEIGHT - 1;
	while (i < SPECTRO_HEIGHT && i >= 0)
	{
		j = 0;
		if (fdf->camera.rotation_y > 0.0)
			j = AUDIO_BUFFER_SIZE / 2 - 1;
		while (j < AUDIO_BUFFER_SIZE / 2 && j >= 0)
		{
			a = project_spectro_pts(i, j);
			if (j < AUDIO_BUFFER_SIZE / 2 - 1)
			{
				b = project_spectro_pts(i, j + 1);
				draw_line(a, b, fdf->img_data);
			}
			if (i < SPECTRO_HEIGHT - 1)
			{
				b = project_spectro_pts(i + 1, j);
				draw_line(a, b, fdf->img_data);
			}
			j += -2 * (fdf->camera.rotation_y > 0) + 1;
		}
		i += -2 * (fdf->camera.rotation_x > 0) + 1;
	}
	mlx_put_image_to_window(fdf->mlx, fdf->win, fdf->img, 0, 0);
	//debug
	if (fdf->debug > 0.0)
	{
		mlx_string_put(fdf->mlx, fdf->win, 10, 10, 0xFFFFFF, "debug: ");
		mlx_string_put(fdf->mlx, fdf->win, 70, 10, 0xFFFFFF, ft_dtoa(fdf->debug, 2));
		mlx_string_put(fdf->mlx, fdf->win, 10, 30, 0xFFFFFF, "volume: ");
		mlx_string_put(fdf->mlx, fdf->win, 80, 30, 0xFFFFFF, ft_dtoa(fdf->audio.volume, 2));
	}
}

