/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   port_audio.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:22:40 by macauchy          #+#    #+#             */
/*   Updated: 2025/03/25 13:37:55 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static double	ftmax(double a, double b)
{
	return (a > b ? a : b);
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
	int		bar_size;

	(void)output;
	(void)time_info;
	(void)status_flags;
	audio = (t_audio *)user_data;
	in = (const float *)input;
	i = 0;
	vol_l = 0;
	vol_r = 0;
	bar_size = 100;
	while (i < frames)
	{
		audio->buffer[i] = in[i];
		i++;
	}
	for (int i = 0; i < AUDIO_BUFFER_SIZE * 2; i+=2)
	{
		vol_l = ftmax(vol_l, fabs(in[i]));
		vol_r = ftmax(vol_r, fabs(in[i + 1]));
	}
	printf("\r");
	for (int i = 0; i < bar_size; i++)
	{
		float	ratio = i / (float)bar_size;

		if (ratio <= vol_l && ratio <= vol_r)
			printf("█");
		else if (ratio <= vol_l)
			printf("▀");
		else if (ratio <= vol_r)
			printf("▄");
		else
			printf(" ");
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
		}
		printf("	device index: %d\n", selectedDevice);
		printf("	sample rate: %f\n", deviceInfo->defaultSampleRate);
		printf("	input channels: %d\n", deviceInfo->maxInputChannels);
		printf("	output channels: %d\n", deviceInfo->maxOutputChannels);
		printf("	input latency: %f\n", deviceInfo->defaultLowInputLatency);
		i++;
	}
	if (selectedDevice < 0)
	{
		ft_putstr_fd("Error: Virtual input device not found\n", 2);
		exit(1);
	}
	parameters.device = 0;
	parameters.channelCount = 2;
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
	update_map_from_audio(fdf);
	mlx_clear_window(fdf->mlx, fdf->win);
	draw_map();
	return (0);
}

