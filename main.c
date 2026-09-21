/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 16:37:15 by drezan            #+#    #+#             */
/*   Updated: 2026/09/21 14:02:39 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "simulation.h"
#include <string.h>

t_sim_param	*parser(int argc, char **argv)
{
	t_sim_param	*sim_param;

	if (argc != 9)
		return (NULL);
	if (atoi(argv[1]) < 1)
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
	gettimeofday(&sim_param->sim_start, NULL);
	return (sim_param);
}

int	main(int argc, char **argv)
{
	t_coder		**coders;
	t_sim_param	*sim_param;
	t_sim_coder	*sim_coders;
	pthread_t	*threads;
	int			n;

	sim_param = parser(argc, argv);
	if (!sim_param)
	{
		printf("Unable to parse command line arguments. ");
		printf("Exiting the program...\n");
		return (0);
	}
	printf("Starting program...\n");
	coders = init_coders(sim_param);
	if (!coders)
	{
		printf("Initializing of coders failed.\n");
		return (0);
	}
	n = sim_param->number_of_coders;
	threads = malloc(sizeof(pthread_t) * n);
	sim_coders = malloc(sizeof(t_sim_coder) * n);
	for (int i = 0; i < n; i += 2)
	{
		sim_coders[i].coder = coders[i];
		sim_coders[i].sim_param = sim_param;
		pthread_create(&threads[i], NULL, coder_run, &sim_coders[i]);
	}
	usleep(2000);
	for (int i = 1; i < n; i += 2)
	{
		sim_coders[i].coder = coders[i];
		sim_coders[i].sim_param = sim_param;
		pthread_create(&threads[i], NULL, coder_run, &sim_coders[i]);
	}
	for (int i = 0; i < n; i += 1)
		pthread_join(threads[i], NULL);

	free(threads);
	free(sim_coders);
	return (0);
}
