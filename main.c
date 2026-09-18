/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:13:13 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/18 18:46:47 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (init_config(&sim, argc, argv))
		return (1);
	if (init_simulation(&sim))
		return (1);
	create_threads(&sim, print_output);
	pthread_create(&sim.monitor, NULL, monitor_routine, &sim);
	finish_thread(&sim);
	destroy_mutexes(&sim);
	free_threads(&sim);
	return (0);
}
