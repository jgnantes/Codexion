/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 15:17:55 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 15:19:15 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

void	increment_compile_count(t_coder *coder)
{
	pthread_mutex_lock(&coder->data->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->data->state_mutex);
}

int	get_compile_count(t_coder *coder)
{
	int	compile_count;

	pthread_mutex_lock(&coder->data->state_mutex);
	compile_count = coder->compile_count;
	pthread_mutex_unlock(&coder->data->state_mutex);
	return (compile_count);
}
