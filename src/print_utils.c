/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:30:14 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 21:46:10 by jnantes-         ###   ########.fr       */
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

void	print_state(t_coder *coder, char *message)
{
	if (is_stopped(coder->data))
		return ;
	print_log(coder, message);
}

void	*print_output(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->data)
		&& get_compile_count(
			coder) < coder->data->config->nbr_compiles_required)
	{
		if (compile_step(coder))
			return (NULL);
		if (is_stopped(coder->data))
			return (NULL);
		print_state(coder, "is debugging");
		msleep(coder->data->config->time_to_debug);
		print_state(coder, "is refactoring");
		msleep(coder->data->config->time_to_refactor);
	}
	return (NULL);
}

int	compile_step(t_coder *coder)
{
	if (take_dongles(coder))
		return (1);
	set_last_compile_start(coder);
	print_state(coder, "is compiling");
	msleep(coder->data->config->time_to_compile);
	increment_compile_count(coder);
	release_dongles(coder);
	return (0);
}
