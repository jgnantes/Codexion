/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:04:48 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/13 23:24:15 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void allocate(pthread_t *threads, t_coder *coders, int n, void *f(void *))
{
	int i;
		
	i = 0;
	while (i < n)
	{
		coders[i].id = i + 1;
		pthread_create(&threads[i], NULL, f, &coders[i]);
		i++;
	}
}

void	finish_thread(pthread_t *threads, t_coder *coders, int n)
{
	int	i;

	i = 0;
	while (i < n)
		pthread_join(threads[i++], NULL);
	free_threads(threads, coders);
}

void free_threads(pthread_t *threads, t_coder *coders)
{
	free(threads);
	free(coders);
}
