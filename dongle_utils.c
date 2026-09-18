/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:41:04 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/18 19:01:13 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	lock_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	if (is_stopped(coder->data))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	print_log(coder, "has taken a dongle");
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
	{
		if (lock_dongle(coder, coder->left))
			return (1);
		while (!is_stopped(coder->data))
			msleep(1);
		pthread_mutex_unlock(&coder->left->mutex);
		return (1);
	}
	get_dongle_order(coder, &first, &second);
	if (lock_dongle(coder, first))
		return (1);
	if (lock_dongle(coder, second))
	{
		pthread_mutex_unlock(&first->mutex);
		return (1);
	}
	return (0);
}

void	release_dongles(t_coder *coder)
{
	if (coder->left == coder->right)
	{
		pthread_mutex_unlock(&coder->left->mutex);
		return ;
	}
	pthread_mutex_unlock(&coder->right->mutex);
	pthread_mutex_unlock(&coder->left->mutex);
}
