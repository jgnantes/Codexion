/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:13:13 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/13 18:08:37 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*print_test(void *arg)
{
	t_coder	*coders;

	coders = (t_coder *)arg;
	printf("Coder %d started\n", coders->id);
	sleep(1);
	printf("Coder %d continues\n", coders->id);
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t	*threads;
	t_coder		*coders;
	t_data		data;
	int			n;

	if (argc != 2)
		return (1);
	n = atoi(argv[1]);
	if (n <= 0)
		return (1);
	threads = malloc(sizeof(pthread_t) * n);
	coders = malloc(sizeof(t_coder) * n);
	if (!threads || !coders)
		return (free_threads(threads, coders), 1);
	pthread_mutex_init(&data.log_mutex, NULL);
	allocate(threads, coders, n, print_test);
	finish_thread(threads, coders, n);
	pthread_mutex_destroy(&data.log_mutex);
	return (0);
}
