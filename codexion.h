/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:17:03 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/14 22:20:10 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>

typedef struct s_config
{
	int	nbr_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
}	t_config;

typedef struct s_data
{
	pthread_mutex_t	log_mutex;
	long			start_time;
	t_config		*config;
}	t_data;

typedef struct s_coder
{
	int		id;
	t_data	*data;
}	t_coder;

typedef struct s_sim
{
	pthread_t	*threads;
	t_coder		*coders;
	t_data		data;
	t_config	config;
}	t_sim;

/*memory utils*/
void	create_threads(t_sim *sim, void *f(void *));
void	finish_thread(t_sim *sim);
void	free_threads(t_sim *sim);

/*system utils*/
long	get_time_ms(void);
void	msleep(long milliseconds);

/*initialization utils*/
int	init_config(t_sim *sim, int argc, char **argv);
int	init_simulation(t_sim *sim);

/*parsing utils*/
int	is_digit(char c);
int	parsing_atoi(char *arg);
