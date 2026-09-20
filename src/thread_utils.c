/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:04:48 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 20:47:30 by jnantes-         ###   ########.fr       */
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
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].queue.mutex);
		pthread_cond_destroy(&sim->dongles[i].queue.cond);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	pthread_mutex_destroy(&sim->data.state_mutex);
	pthread_mutex_destroy(&sim->data.log_mutex);
}

int	free_threads(t_sim *sim)
{
	int	i;

	if (!sim)
		return (0);
	free(sim->threads);
	free(sim->coders);
	i = 0;
	while (i < sim->config.nbr_coders)
	{
		free(sim->dongles[i].queue.items);
		i++;
	}
	free(sim->dongles);
	return (1);
}
