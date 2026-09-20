/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   startup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jnantes- <jnantes-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 21:57:25 by jnantes-          #+#    #+#             */
/*   Updated: 2026/09/19 21:37:55 by jnantes-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_config(t_sim *sim, int argc, char **argv)
{
	if (argc != 9)
		return (1);
	sim->config.nbr_coders = parsing_atoi(argv[1]);
	sim->config.time_to_burnout = parsing_atoi(argv[2]);
	sim->config.time_to_compile = parsing_atoi(argv[3]);
	sim->config.time_to_debug = parsing_atoi(argv[4]);
	sim->config.time_to_refactor = parsing_atoi(argv[5]);
	sim->config.nbr_compiles_required = parsing_atoi(argv[6]);
	sim->config.dongle_cooldown = parsing_atoi(argv[7]);
	sim->config.scheduler = argv[8];
	if (sim->config.nbr_coders <= 0
		|| sim->config.time_to_burnout <= 0
		|| sim->config.time_to_compile <= 0
		|| sim->config.time_to_debug <= 0
		|| sim->config.time_to_refactor <= 0
		|| sim->config.nbr_compiles_required <= 0
		|| sim->config.dongle_cooldown < 0)
		return (1);
	if (strcmp(sim->config.scheduler, "fifo") != 0
		&& strcmp(sim->config.scheduler, "edf") != 0)
		return (1);
	return (0);
}

int	init_simulation(t_sim *sim)
{
	int	i;

	sim->threads = malloc(sizeof(pthread_t) * sim->config.nbr_coders);
	sim->coders = malloc(sizeof(t_coder) * sim->config.nbr_coders);
	sim->dongles = malloc(sizeof(t_dongle) * sim->config.nbr_coders);
	if (!sim->threads || !sim->coders || !sim->dongles)
		return (free_threads(sim));
	memset(sim->dongles, 0, sizeof(t_dongle) * sim->config.nbr_coders);
	pthread_mutex_init(&sim->data.log_mutex, NULL);
	pthread_mutex_init(&sim->data.state_mutex, NULL);
	sim->data.stop = 0;
	i = 0;
	while (i < sim->config.nbr_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].cooldown_until = 0;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		if (init_request_queue(&sim->dongles[i].queue, sim->config.nbr_coders))
			return (free_threads(sim));
		i++;
	}
	sim->data.start_time = get_time_ms();
	sim->data.config = &sim->config;
	return (0);
}

int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	parsing_atoi(char *arg)
{
	int		i;
	long	nbr;

	i = 0;
	nbr = 0;
	if (!arg[0])
		return (-1);
	while (arg[i])
	{
		if (!is_digit(arg[i]))
			return (-1);
		nbr = (nbr * 10) + (arg[i] - '0');
		if (nbr > INT_MAX)
			return (-1);
		i++;
	}
	return ((int)nbr);
}
