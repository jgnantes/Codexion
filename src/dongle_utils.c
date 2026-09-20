/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:41:04 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 21:45:01 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_dongle(t_coder *coder, t_dongle *dongle)
{
	long	cooldown_until;
	long	now;

	pthread_mutex_lock(&dongle->mutex);
	cooldown_until = dongle->cooldown_until;
	pthread_mutex_unlock(&dongle->mutex);
	now = get_time_ms();
	if (cooldown_until > now)
		msleep(cooldown_until - now);
	pthread_mutex_lock(&dongle->mutex);
	if (is_stopped(coder->data))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	return (0);
}

void	get_dongle_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left->id < coder->right->id)
	{
		*first = coder->left;
		*second = coder->right;
	}
	else
	{
		*first = coder->right;
		*second = coder->left;
	}
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left == coder->right)
		take_single_dongle(coder);
	get_dongle_order(coder, &first, &second);
	if (wait_pair_turn(coder, first, second))
		return (1);
	if (check_dongle(coder, first))
		return (1);
	if (check_dongle(coder, second))
	{
		pthread_mutex_unlock(&first->mutex);
		return (1);
	}
	print_state(coder, "has taken a dongle");
	print_state(coder, "has taken a dongle");
	return (0);
}

int	take_single_dongle(t_coder *coder)
{
	if (check_dongle(coder, coder->left))
		return (1);
	print_state(coder, "has taken a dongle");
	while (!is_stopped(coder->data))
		msleep(1);
	pthread_mutex_unlock(&coder->left->mutex);
	return (1);
}

void	release_dongles(t_coder *coder)
{
	long	cooldown_until;

	cooldown_until = get_time_ms() + coder->data->config->dongle_cooldown;
	if (coder->left == coder->right)
	{
		coder->left->cooldown_until = cooldown_until;
		pthread_mutex_unlock(&coder->left->mutex);
		return ;
	}
	coder->right->cooldown_until = cooldown_until;
	pthread_mutex_unlock(&coder->right->mutex);
	coder->left->cooldown_until = cooldown_until;
	pthread_mutex_unlock(&coder->left->mutex);
}
