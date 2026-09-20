/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 18:53:19 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 21:16:05 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_request	create_request(t_coder *coder)
{
	t_request	request;

	request.coder = coder;
	request.arrival_time = get_time_ms();
	request.deadline = get_last_compile_start(coder)
		+ coder->data->config->time_to_burnout;
	return (request);
}

int	compare_fifo(t_request a, t_request b)
{
	if (a.arrival_time < b.arrival_time)
		return (1);
	if (a.arrival_time > b.arrival_time)
		return (0);
	return (a.coder->id < b.coder->id);
}

int	compare_edf(t_request a, t_request b)
{
	if (a.deadline < b.deadline)
		return (1);
	if (a.deadline > b.deadline)
		return (0);
	if (a.arrival_time < b.arrival_time)
		return (1);
	if (a.arrival_time > b.arrival_time)
		return (0);
	return (a.coder->id < b.coder->id);
}

int	choose_scheduler(t_request a, t_request b, char *scheduler)
{
	if (strcmp(scheduler, "edf") == 0)
		return (compare_edf(a, b));
	return (compare_fifo(a, b));
}
