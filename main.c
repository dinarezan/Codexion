/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 16:37:15 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 18:34:17 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "monitor.h"
#include "parsing_validation.h"
#include "simulation.h"

t_sim_param	*parser(int argc, char **argv)
{
	t_sim_param	*sim_param;

	if (argc != 9)
		return (NULL);
	if (arg_check(argv) < 0)
		return (NULL);
	sim_param = malloc(sizeof(t_sim_param));
	sim_param->number_of_coders = atoi(argv[1]);
	sim_param->time_to_burnout = atoi(argv[2]);
	sim_param->time_to_compile = atoi(argv[3]);
	sim_param->time_to_debug = atoi(argv[4]);
	sim_param->time_to_refactor = atoi(argv[5]);
	sim_param->number_of_compiles_required = atoi(argv[6]);
	sim_param->dongle_cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		sim_param->scheduler = 0;
	else if (strcmp(argv[8], "edf") == 0)
		sim_param->scheduler = 1;
	else
		return (free(sim_param), NULL);
	sim_param->sim_stop = 0;
	pthread_mutex_init(&sim_param->stop_lock, NULL);
	return (sim_param);
}

void	free_all(t_coder **coders, t_dongle **dongles, t_sim_param *sim_param)
{
	free_dongles(dongles);
	free_coders(coders);
	free(sim_param);
}

void	create_threads(t_sim_coder *sim_coders, t_sim_param *sim_param,
		t_coder **coders, pthread_t *threads)
{
	int	i;
	int	n;

	n = sim_param->number_of_coders;
	i = 0;
	while (i < n)
	{
		sim_coders[i].coder = coders[i];
		sim_coders[i].sim_param = sim_param;
		pthread_create(&threads[i], NULL, coder_run, &sim_coders[i]);
		i += 2;
	}
	usleep(sim_param->time_to_compile / 2 * 1000);
	i = 1;
	while (i < n)
	{
		sim_coders[i].coder = coders[i];
		sim_coders[i].sim_param = sim_param;
		pthread_create(&threads[i], NULL, coder_run, &sim_coders[i]);
		i += 2;
	}
}

void	start_simulation(t_coder **coders, int n, t_sim_param *sim_param)
{
	t_sim_coder	*sim_coders;
	pthread_t	*threads;
	pthread_t	*monitor;
	int			i;

	printf("Starting program...\n");
	gettimeofday(&sim_param->sim_start, NULL);
	set_last_compilation_time(coders, sim_param);
	monitor = init_monitor(coders, sim_param);
	if (!monitor)
		return ;
	threads = malloc(sizeof(pthread_t) * n);
	sim_coders = malloc(sizeof(t_sim_coder) * n);
	create_threads(sim_coders, sim_param, coders, threads);
	pthread_join(*monitor, NULL);
	i = 0;
	while (i < n)
		pthread_join(threads[i++], NULL);
	pthread_mutex_destroy(&sim_param->stop_lock);
	free(monitor);
	free(threads);
	free(sim_coders);
	return ;
}

int	main(int argc, char **argv)
{
	t_coder		**coders;
	t_dongle	**dongles;
	t_sim_param	*sim_param;

	sim_param = parser(argc, argv);
	if (!sim_param)
	{
		printf("Unable to parse command line arguments. ");
		return (0);
	}
	dongles = init_dongles(sim_param);
	if (!dongles)
		return (free(sim_param), 0);
	coders = init_coders(sim_param, dongles);
	if (!coders)
		return (free(sim_param), free_dongles(dongles), 0);
	start_simulation(coders, sim_param->number_of_coders, sim_param);
	free_all(coders, dongles, sim_param);
	return (0);
}
