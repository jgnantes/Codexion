/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 21:05:36 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 21:08:17 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	sift_up(t_queue *queue, int index, char *scheduler)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!choose_scheduler(queue->items[index],
				queue->items[parent], scheduler))
			return ;
		swap_requests(&queue->items[index], &queue->items[parent]);
		index = parent;
	}
}

void	sift_down(t_queue *queue, int index, char *scheduler)
{
	int	left;
	int	right;
	int	best;

	while (1)
	{
		left = (index * 2) + 1;
		right = (index * 2) + 2;
		best = index;
		if (left < queue->size
			&& choose_scheduler(queue->items[left], queue->items[best],
				scheduler))
			best = left;
		if (right < queue->size
			&& choose_scheduler(queue->items[right], queue->items[best],
				scheduler))
			best = right;
		if (best == index)
			return ;
		swap_requests(&queue->items[index], &queue->items[best]);
		index = best;
	}
}

int	heap_push(t_queue *queue, t_request request, char *scheduler)
{
	if (queue->size >= queue->capacity)
		return (1);
	queue->items[queue->size] = request;
	queue->size++;
	sift_up(queue, queue->size - 1, scheduler);
	return (0);
}

void	heap_pop(t_queue *queue, char *scheduler)
{
	if (queue->size == 0)
		return ;
	queue->items[0] = queue->items[queue->size - 1];
	queue->size--;
	sift_down(queue, 0, scheduler);
}
