/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:04:48 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/16 23:05:31 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_threads(t_sim *sim, void *f(void *))
{
	int	i;

	i = 0;
	while (i < sim->config.nbr_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].data = &sim->data;
		sim->coders[i].last_compile_start = sim->data.start_time;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % sim->config.nbr_coders];
		pthread_create(&sim->threads[i], NULL, f, &sim->coders[i]);
		i++;
	}
}

void	finish_thread(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nbr_coders)
	{
		pthread_join(sim->threads[i], NULL);
		i++;
	}
}

void	free_threads(t_sim *sim)
{
	free(sim->threads);
	free(sim->coders);
	free(sim->dongles);
}

void	destroy_mutexes(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.nbr_coders)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&sim->data.state_mutex);
	pthread_mutex_destroy(&sim->data.log_mutex);
}