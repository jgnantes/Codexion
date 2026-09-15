/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:13:13 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/13 18:08:37 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_log(t_coder *coder, char *message)
{
	pthread_mutex_lock(&coder->data->log_mutex);
	printf("%ld Coder %d %s\n",
		get_time_ms() - coder->data->start_time,
		coder->id,
		message);
	pthread_mutex_unlock(&coder->data->log_mutex);
}

void	*print_test(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	print_log(coder, "is compiling");
	msleep(coder->data->config->time_to_compile);
	print_log(coder, "is debugging");
	msleep(coder->data->config->time_to_debug);
	print_log(coder, "is refactoring");
	msleep(coder->data->config->time_to_refactor);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	if (init_config(&sim, argc, argv))
		return (1);
	if (init_simulation(&sim))
		return (1);
	create_threads(&sim, print_test);
	finish_thread(&sim);
	pthread_mutex_destroy(&sim.data.log_mutex);
	free_threads(&sim);
	return (0);
}
