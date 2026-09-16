/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 16:37:15 by drezan            #+#    #+#             */
/*   Updated: 2026/09/16 15:06:32 by drezan           ###   ########.fr       */
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
	sim_param->coders = coders;
	printf("Coders initialized.\n");
	for (int i = 0; i < 10; i++)
	{
		coder_run(sim_param, coders[i]);
	}
	return (0);
}
