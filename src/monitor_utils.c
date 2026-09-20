/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 18:30:45 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 21:47:56 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!is_stopped(&sim->data))
	{
		if (all_coders_finished(sim))
		{
			set_stop(sim);
			return (NULL);
		}
		if (check_burnout(sim))
			return (NULL);
		msleep(1);
	}
	return (NULL);
}

int	is_stopped(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->state_mutex);
	stop = data->stop;
	pthread_mutex_unlock(&data->state_mutex);
	return (stop);
}

void	set_stop(t_sim *sim)
{
	int	i;

	pthread_mutex_lock(&sim->data.state_mutex);
	sim->data.stop = 1;
	pthread_mutex_unlock(&sim->data.state_mutex);
	i = 0;
	while (i < sim->config.nbr_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].queue.mutex);
		pthread_cond_broadcast(&sim->dongles[i].queue.cond);
		pthread_mutex_unlock(&sim->dongles[i].queue.mutex);
		i++;
	}
}

int	check_burnout(t_sim *sim)
{
	int		i;
	long	now;
	long	last;

	i = 0;
	while (i < sim->config.nbr_coders)
	{
		now = get_time_ms();
		last = get_last_compile_start(&sim->coders[i]);
		if (now - last > sim->config.time_to_burnout)
		{
			set_stop(sim);
			print_log(&sim->coders[i], "burned out");
			return (1);
		}
		i++;
	}
	return (0);
}

int	all_coders_finished(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nbr_coders)
	{
		if (get_compile_count(
				&sim->coders[i]) < sim->config.nbr_compiles_required)
			return (0);
		i++;
	}
	return (1);
}
