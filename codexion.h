/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:17:03 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/18 19:08:18 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <limits.h>

typedef struct s_config
{
	int		nbr_coders;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		nbr_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_config;

typedef struct s_data
{
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	state_mutex;
	long			start_time;
	int				stop;
	t_config		*config;
}	t_data;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				id;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			compile_count;
	long		last_compile_start;
	t_dongle	*left;
	t_dongle	*right;
	t_data		*data;
}	t_coder;

typedef struct s_sim
{
	pthread_t	*threads;
	pthread_t	monitor;
	t_coder		*coders;
	t_dongle	*dongles;
	t_data		data;
	t_config	config;
}	t_sim;

/*thread utils*/
void	create_threads(t_sim *sim, void *f(void *));
void	finish_thread(t_sim *sim);
void	free_threads(t_sim *sim);
void	destroy_mutexes(t_sim *sim);
void	*monitor_routine(void *arg);

/*time utils*/
long	get_time_ms(void);
void	msleep(long milliseconds);
void	set_last_compile_start(t_coder *coder);
long	get_last_compile_start(t_coder *coder);

/*initialization utils*/
int		init_config(t_sim *sim, int argc, char **argv);
int		init_simulation(t_sim *sim);
int		is_digit(char c);
int		parsing_atoi(char *arg);

/*print utils*/
void	print_log(t_coder *coder, char *message);
void	*print_output(void *arg);
int		compile_step(t_coder *coder);

/*dongle utils*/
int		take_dongles(t_coder *coder);
void	get_dongle_order(t_coder *coder, t_dongle **first, t_dongle **second);
int		lock_dongle(t_coder *coder, t_dongle *dongle);
void	release_dongles(t_coder *coder);

/*monitor utils*/
int		is_stopped(t_data *data);
void	set_stop(t_data *data);
int		check_burnout(t_sim *sim);
int		all_coders_finished(t_sim *sim);
