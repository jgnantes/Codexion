/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 16:17:03 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 21:46:46 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <limits.h>

typedef struct s_config		t_config;
typedef struct s_data		t_data;
typedef struct s_coder		t_coder;
typedef struct s_request	t_request;
typedef struct s_queue		t_queue;
typedef struct s_dongle		t_dongle;
typedef struct s_sim		t_sim;

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

typedef struct s_coder
{
	int			id;
	int			compile_count;
	long		last_compile_start;
	t_dongle	*left;
	t_dongle	*right;
	t_data		*data;
}	t_coder;

typedef struct s_request
{
	t_coder	*coder;
	long	arrival_time;
	long	deadline;
}	t_request;

typedef struct s_queue
{
	t_request		*items;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				size;
	int				capacity;
}	t_queue;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_queue			queue;
	long			cooldown_until;
	int				id;
}	t_dongle;

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
void		create_threads(t_sim *sim, void *f(void *));
void		finish_thread(t_sim *sim);
int			free_threads(t_sim *sim);

/*time utils*/
long		get_time_ms(void);
void		msleep(long milliseconds);

/*startup utils*/
int			init_config(t_sim *sim, int argc, char **argv);
int			init_simulation(t_sim *sim);
int			is_digit(char c);
int			parsing_atoi(char *arg);

/*print utils*/
void		print_log(t_coder *coder, char *message);
void		print_state(t_coder *coder, char *message);
void		*print_output(void *arg);
int			compile_step(t_coder *coder);

/*dongle utils*/
int			check_dongle(t_coder *coder, t_dongle *dongle);
void		get_dongle_order(
				t_coder *coder, t_dongle **first, t_dongle **second);
int			take_dongles(t_coder *coder);
int			take_single_dongle(t_coder *coder);
void		release_dongles(t_coder *coder);

/*monitor utils*/
void		*monitor_routine(void *arg);
int			is_stopped(t_data *data);
void		set_stop(t_sim *sim);
int			check_burnout(t_sim *sim);
int			all_coders_finished(t_sim *sim);

/*state utils*/
void		set_last_compile_start(t_coder *coder);
long		get_last_compile_start(t_coder *coder);
void		increment_compile_count(t_coder *coder);
int			get_compile_count(t_coder *coder);

/*scheduler utils*/
t_request	create_request(t_coder *coder);
int			compare_fifo(t_request a, t_request b);
int			compare_edf(t_request a, t_request b);
int			choose_scheduler(t_request a, t_request b, char *scheduler);

/*request utils*/
int			init_request_queue(t_queue *queue, int capacity);
int			push_request(t_queue *queue, t_request request, char *scheduler);
int			wait_turn(t_coder *coder, t_dongle *dongle);
int			wait_pair_turn(t_coder *coder, t_dongle *first,
				t_dongle *second);
void		swap_requests(t_request *a, t_request *b);

/*heap utils*/
void		sift_up(t_queue *queue, int index, char *scheduler);
void		sift_down(t_queue *queue, int index, char *scheduler);
int			heap_push(t_queue *queue, t_request request, char *scheduler);
void		heap_pop(t_queue *queue, char *scheduler);
