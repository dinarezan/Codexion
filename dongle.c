/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 12:20:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 15:09:02 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"
#include "simulation.h"

void	free_dongles(t_dongle **dongles)
{
	int	i;

	i = 0;
	if (!dongles)
		return ;
	while (dongles[i])
	{
		pthread_mutex_destroy(&dongles[i]->dongle);
		pthread_cond_destroy(&dongles[i]->condition);
		free(dongles[i]);
		i++;
	}
	free(dongles);
}

t_dongle	**init_dongles(t_sim_param *sim_param)
{
	t_dongle	**dongles;
	int			n;
	int			i;

	i = 0;
	n = sim_param->number_of_coders;
	dongles = (t_dongle **)malloc((n + 1) * sizeof(t_dongle *));
	if (!dongles)
		return (NULL);
	while (i < n)
	{
		dongles[i] = malloc(sizeof(t_dongle));
		if (!dongles[i])
			return (free_dongles(dongles), NULL);
		pthread_mutex_init(&dongles[i]->dongle, NULL);
		pthread_cond_init(&dongles[i]->condition, NULL);
		dongles[i]->id = i;
		dongles[i]->ticket_counter = 0;
		dongles[i]->current_turn = 0;
		dongles[i]->last_compile.tv_sec = 0;
		dongles[i]->last_compile.tv_usec = 0;
		i++;
	}
	dongles[i] = NULL;
	return (dongles);
}
