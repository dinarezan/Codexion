/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 12:20:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/18 15:02:12 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"
#include "simulation.h"

t_dongle	**init_dongles(t_sim_param *sim_param)
{
	t_dongle	**dongles;
	int			n;

	n = sim_param->number_of_coders;
	dongles = (t_dongle **)malloc(n * sizeof(t_dongle *));
	if (!dongles)
		return (NULL);
	for (int i = 0; i < n; i++)
	{
		dongles[i] = malloc(sizeof(t_dongle));
		if (!dongles[i])
			return (NULL);
		pthread_mutex_init(&dongles[i]->dongle, NULL);
		pthread_cond_init(&dongles[i]->condition, NULL);
		dongles[i]->id = i;
		dongles[i]->ticket_counter = 0;
		dongles[i]->current_turn = 0;
		dongles[i]->last_compile = sim_param->sim_start;
	}
	return (dongles);
}
