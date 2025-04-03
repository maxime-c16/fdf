/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bpm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macauchy <macauchy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:03:22 by macauchy          #+#    #+#             */
/*   Updated: 2025/04/02 14:33:08 by macauchy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

static void	autocorrelate(const double *signal, int size, double *result)
{
	int	i;
	int	lag;

	lag = 0;
	while (lag < size)
	{
		result[lag] = 0.0;
		i = 0;
		while (i < size - lag)
		{
			result[lag] += signal[i] * signal[i + lag];
			i++;
		}
		lag++;
	}

}

static int	find_first_peak(const double *ac, int size, int min_lag, int max_lag)
{
	int		i;
	int		peak_index;
	double	peak_value;

	peak_index = 0;
	peak_value = 0.0;
	i = min_lag;
	while (i < size && i <= max_lag)
	{
		if (i > 0 && i < size - 1)
		{
			if (ac[i] > ac[i - 1] && ac[i] > ac[i + 1] && ac[i] > peak_value)
			{
				peak_index = i;
				peak_value = ac[i];
			}
		}
		i++;
	}
	return (peak_index);
}

static double	lag_to_bpm(double lag, double update_interval)
{
	if (lag <= 0.0)
		return (0.0);
	return (60.0 / (lag * update_interval));
}

static double	compute_spectral(t_audio *audio, double *prev)
{
	int		i;
	double	flux;
	double	diff;

	flux = 0.0;
	i = 0;
	while (i < AUDIO_BUFFER_SIZE / 2 + 1)
	{
		diff = audio->fft_out[i] - prev[i];
		if (diff > 0.0)
			flux += diff;
		i++;
	}
	return (flux);
}

static double	compute_bpm_from_low_fft(t_audio *audio)
{
	int		n;
	double	*temp_in;
	double	*temp_out;
	fftw_plan	plan;
	int		i;
	double	mag;
	double	max_mag;
	int		max_bin;
	double	freq, bpm;

	/* n is the size of the region (spectro_size) we care about */
	n = AUDIO_BUFFER_SIZE / 2;
	temp_in = (double *)malloc(sizeof(double) * n);
	temp_out = (double *)malloc(sizeof(double) * n);
	if (!temp_in || !temp_out)
	{
		dprintf(2, "Error: malloc failed in compute_bpm_from_low_fft\n");
		exit(1);
	}
	plan = fftw_plan_r2r_1d(n, temp_in, temp_out, FFTW_R2HC, FFTW_ESTIMATE);
	if (!plan)
	{
		dprintf(2, "Error: fftw_plan_r2r_1d failed in compute_bpm_from_low_fft\n");
		exit(1);
	}
	/* Copy the low-frequency region from the first FFT output,
	   starting at index 1 to ignore DC */
	i = 1;
	while (i < n && (1 + i) < AUDIO_BUFFER_SIZE / 2 + 1)
	{
		temp_in[i] = fabs(audio->fft_out[1 + i]);
		i++;
	}
	/* Ensure DC (index 0) is zeroed */
	temp_in[0] = 0.0;
	/* Execute the second FFT on the low-frequency region */
	fftw_execute(plan);
	/* Find the maximum magnitude in the second FFT output.
	   We assume the signal is real, so use the R2HC format:
	   For bin 0 and (if even) bin n/2, value is real;
	   For bins 1..n/2-1, imaginary part is in temp_out[n-i]. */
	max_mag = 0.0;
	max_bin = 1;
	mag = 0.0;
	i = 1;
	while (i < n)
	{
		if (i == 0 || (n % 2 == 0 && i == n / 2))
			mag = fabs(temp_out[i]);
		else if (i < n / 2)
			mag = sqrt(temp_out[i] * temp_out[i] +
					temp_out[n - i] * temp_out[n - i]);
		else
			mag = 0.0;
		if (mag > max_mag)
		{
			max_mag = mag;
			max_bin = i;
		}
		i++;
	}
	/* Convert the bin index to frequency in Hz:
	   bin_resolution = SAMPLE_RATE / n,
	   then BPM = frequency * 60 */
	freq = (double)max_bin * (EFFECTIVE_FREQ_RANGE / (double)n);
	// printf("Max bin: %d, max_mag: %f\n", max_bin, max_mag);
	// printf("Max frequency: %f Hz\n", freq);
	// printf("Low BPM: %f\n", freq * 60.0);
	bpm = freq * 60.0;
	fftw_destroy_plan(plan);
	free(temp_in);
	free(temp_out);
	return (bpm);
}


void	update_bpm(t_fdf *fdf, double current_time)
{
	t_bpm 	*bpm;
	t_audio *audio;
	double	flux;
	double	interval;
	double	sum;
	int		i;
	double	bpm_low;
	double	ac_result[FLUX_BUFFER_SIZE];
	int		peak_lag;
	double	bpm_est;
	double	update_interval;

	bpm = &fdf->bpm_data;
	audio = &fdf->audio;
	if (bpm->last_update > 0.0)
	{
		update_interval = current_time - bpm->last_update;
		bpm->last_update = current_time;
	}
	else
	{
		bpm->last_update = current_time;
		update_interval = 0.0016;
	}
	autocorrelate(bpm->flux_history, FLUX_BUFFER_SIZE, ac_result);
	peak_lag = find_first_peak(ac_result, FLUX_BUFFER_SIZE, 14 + fdf->debug, 19);
	bpm_est = lag_to_bpm(peak_lag, 0.02);
	bpm->bpm = bpm_est;
	flux = compute_spectral(audio, bpm->flux_prev);
	bpm->smooth_flux = (bpm->beat_count == 0) ? flux :
		(0.2 * flux) + ((1.0 - 0.2) * bpm->smooth_flux);
	// flux /= (double)(audio->volume + 0.0000001);
	flux = bpm->smooth_flux / (double)(audio->volume + 0.0000001);
	i = 0;
	while (i < AUDIO_BUFFER_SIZE / 2 + 1)
	{
		bpm->flux_prev[i] = audio->fft_out[i];
		i++;
	}
	bpm->flux_history[bpm->index_history] = flux;
	bpm->index_history = (bpm->index_history + 1) % FLUX_BUFFER_SIZE;
	// bpm_low = compute_bpm_from_low_fft(audio);
	// if (flux > 450.0 && (current_time - bpm->last_beat_time) > 0.3)
	// {
	// 	interval = (current_time - bpm->last_beat_time);
	// 	printf("Beat!\n");
	// 	printf("Interval: %.2f\n", interval);
	// 	printf("Flux: %.2f\n", flux);
	// 	bpm->last_beat_time = current_time;
	// 	bpm->beat_intervals[bpm->beat_count % FLUX_BUFFER_SIZE] = interval;
	// 	bpm->beat_count++;
	// 	sum = 0.0;
	// 	i = 0;
	// 	while (i < bpm->beat_count && i < FLUX_BUFFER_SIZE)
	// 	{
	// 		sum += 60.0 / bpm->beat_intervals[i];
	// 		i++;
	// 	}
	// 	if (i > 0)
	// 	{
	// 		bpm->bpm = (double)(sum / (double)(i));
	// 		printf("BPM: %.2f\n", bpm->bpm);
	// 		printf("BPM Low: %.2f\n", bpm_low);
	// 	}
	// }
}
