/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 16:37:15 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 13:49:13 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "simulation.h"
#include "parsing_validation.h"
#include "monitor.h"

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

void free_all(t_coder **coders, t_dongle **dongles, t_sim_param *sim_param, t_sim_coder *sim_coders)
{
	free_dongles(dongles);
	free_coders(coders);
	free(sim_param);
	free(sim_coders);
}

int	main(int argc, char **argv)
{
	t_coder		**coders;
	t_dongle	**dongles;
	t_sim_param	*sim_param;
	t_sim_coder	*sim_coders;
	pthread_t	*threads;
	pthread_t	*monitor;
	int			n;

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
		return (free(sim_param), 0);
	n = sim_param->number_of_coders;
	threads = malloc(sizeof(pthread_t) * n);
	sim_coders = malloc(sizeof(t_sim_coder) * n);
	monitor = init_monitor(coders, sim_param);
	printf("Starting program...\n");
	gettimeofday(&sim_param->sim_start, NULL);
	set_last_compilation_time(coders, sim_param);
	for (int i = 0; i < n; i += 2)
	{
		sim_coders[i].coder = coders[i];
		sim_coders[i].sim_param = sim_param;
		pthread_create(&threads[i], NULL, coder_run, &sim_coders[i]);
	}
	usleep(sim_param->time_to_compile / 2 * 1000);
	for (int i = 1; i < n; i += 2)
	{
		sim_coders[i].coder = coders[i];
		sim_coders[i].sim_param = sim_param;
		pthread_create(&threads[i], NULL, coder_run, &sim_coders[i]);
	}
	pthread_join(*monitor, NULL);
	for (int i = 0; i < n; i += 1)
		pthread_join(threads[i], NULL);
	pthread_mutex_destroy(&sim_param->stop_lock);
	free_all(coders, dongles, sim_param, sim_coders);
	free(monitor);
	free(threads);
	return (0);
}
