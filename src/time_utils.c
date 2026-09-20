/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 21:18:19 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 15:18:18 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timespec	time;

	clock_gettime(CLOCK_MONOTONIC, &time);
	return ((time.tv_sec * 1000) + (time.tv_nsec / 1000000));
}

void	msleep(long milliseconds)
{
	usleep(milliseconds * 1000);
}
