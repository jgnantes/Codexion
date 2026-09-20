/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 19:01:04 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 21:48:23 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_request_queue(t_queue *queue, int capacity)
{
	queue->capacity = capacity;
	queue->size = 0;
	queue->items = malloc(sizeof(t_request) * queue->capacity);
	if (!queue->items)
		return (1);
	pthread_mutex_init(&queue->mutex, NULL);
	pthread_cond_init(&queue->cond, NULL);
	return (0);
}

int	push_request(t_queue *queue, t_request request, char *scheduler)
{
	pthread_mutex_lock(&queue->mutex);
	if (heap_push(queue, request, scheduler))
	{
		pthread_mutex_unlock(&queue->mutex);
		return (1);
	}
	pthread_cond_broadcast(&queue->cond);
	pthread_mutex_unlock(&queue->mutex);
	return (0);
}

int	wait_turn(t_coder *coder, t_dongle *dongle)
{
	t_queue	*queue;

	queue = &dongle->queue;
	if (push_request(queue, create_request(
				coder), coder->data->config->scheduler))
		return (1);
	pthread_mutex_lock(&queue->mutex);
	while (!is_stopped(coder->data))
	{
		if (queue->size > 0 && queue->items[0].coder == coder)
		{
			heap_pop(queue, coder->data->config->scheduler);
			pthread_cond_broadcast(&queue->cond);
			pthread_mutex_unlock(&queue->mutex);
			return (0);
		}
		pthread_cond_wait(&queue->cond, &queue->mutex);
	}
	pthread_mutex_unlock(&queue->mutex);
	return (1);
}

int	wait_pair_turn(t_coder *coder, t_dongle *first, t_dongle *second)
{
	if (wait_turn(coder, first))
		return (1);
	if (wait_turn(coder, second))
		return (1);
	return (0);
}

void	swap_requests(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
