/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 21:18:19 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/16 23:16:23 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timespec	time;

	clock_gettime(CLOCK_MONOTONIC, &time);
	return ((time.tv_sec * 1000) + (time.tv_nsec / 1000000));
}

void	msleep(long milliseconds)
{
	usleep(milliseconds * 1000);
}

void	set_last_compile_start(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->data->state_mutex);
}

long	get_last_compile_start(t_coder *coder)
{
	long	last_compile_start;

	pthread_mutex_lock(&coder->data->state_mutex);
	last_compile_start = coder->last_compile_start;
	pthread_mutex_unlock(&coder->data->state_mutex);
	return (last_compile_start);
}
