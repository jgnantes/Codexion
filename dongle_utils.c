/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:41:04 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/16 20:58:03 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->left == coder->right)
	{
		pthread_mutex_lock(&coder->left->mutex);
		print_log(coder, "has taken a dongle");
		return ;
	}
	if (coder->left->id < coder->right->id)
	{
		first = coder->left;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second = coder->left;
	}
	pthread_mutex_lock(&first->mutex);
	print_log(coder, "has taken a dongle");
	pthread_mutex_lock(&second->mutex);
	print_log(coder, "has taken a dongle");
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
