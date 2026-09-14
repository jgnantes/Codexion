/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:17:03 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/13 23:24:24 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct s_data
{
	pthread_mutex_t	log_mutex;
}	t_data;

typedef struct s_coder
{
	int		id;
	t_data	*data;
}	t_coder;

//memory utils
void	allocate(pthread_t *threads, t_coder *coders, int n, void *f(void *));
void	finish_thread(pthread_t *threads, t_coder *coders, int n);
void	free_threads(pthread_t *threads, t_coder *coders);
