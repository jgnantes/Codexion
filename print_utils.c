/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:30:14 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/16 23:19:20 by jnantes-         ###   ########.fr       */
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

void	*print_output(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (coder->compile_count < coder->data->config->nbr_compiles_required)
	{
		take_dongles(coder);
		set_last_compile_start(coder);
		print_log(coder, "is compiling");
		msleep(coder->data->config->time_to_compile);
		coder->compile_count++;
		release_dongles(coder);
		print_log(coder, "is debugging");
		msleep(coder->data->config->time_to_debug);
		print_log(coder, "is refactoring");
		msleep(coder->data->config->time_to_refactor);
	}
	return (NULL);
}