/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:04:48 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/14 21:53:58 by jnantes-         ###   ########.fr       */
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
		sim->coders[i].data = &sim->data;
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
}
