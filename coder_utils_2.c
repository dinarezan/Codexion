/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/25 17:41:12 by drezan            #+#    #+#             */
/*   Updated: 2026/10/03 17:22:56 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	set_last_compilation_time(t_coder **coders, t_sim_param *sim_param)
{
	int	i;

	i = 0;
	while (coders[i])
	{
		coders[i]->last_compile = sim_param->sim_start;
		coders[i]->time_to_burnout = sim_param->time_to_burnout;
		i++;
	}
}

int	sim_stop(t_sim_param *sim_param)
{
	pthread_mutex_lock(&sim_param->stop_lock);
	if (sim_param->sim_stop == 1)
	{
		pthread_mutex_unlock(&sim_param->stop_lock);
		return (1);
	}
	pthread_mutex_unlock(&sim_param->stop_lock);
	return (0);
}

void	free_coders(t_coder **coders)
{
	int	i;

	i = 0;
	if (!coders)
		return ;
	while (coders[i])
	{
		pthread_mutex_destroy(&coders[i]->state_lock);
		free(coders[i]);
		i++;
	}
	free(coders);
}
