/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 16:12:11 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 12:11:33 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	*monitor(void *args)
{
	int			i;
	int			done;
	t_coder		**coders;
	t_sim_param	*sim_param;

	coders = ((t_sim_coders *)args)->coders;
	sim_param = ((t_sim_coders *)args)->sim_param;
	done = 0;
	while (done < sim_param->number_of_coders)
	{
		i = 0;
		done = 0;
		while (coders[i])
		{
			if (coders[i]->compile_count < sim_param->number_of_compiles_required
				&& time_difference(coders[i]->last_compile) >= sim_param->time_to_burnout)
			{
				printf("%lld %d burned out\n",
					time_difference(sim_param->sim_start), coders[i]->id);
				pthread_mutex_lock(&sim_param->stop_lock);
				sim_param->sim_stop = 1;
				pthread_mutex_unlock(&sim_param->stop_lock);
				return NULL;
			}
			if (coders[i]->compile_count >= sim_param->number_of_compiles_required)
				done++;
			i++;
		}
	}
	return NULL;
}

pthread_t	*init_monitor(t_coder **coders, t_sim_param *sim_param)
{
	pthread_t	 *monitor_thread;
	t_sim_coders *sim_coders;
	
	sim_coders = malloc(sizeof(t_sim_coders));
	sim_coders->coders = coders;
	sim_coders->sim_param = sim_param;
	monitor_thread = malloc(sizeof(pthread_t));
	if (!monitor_thread)
		return (NULL);
	if (pthread_create(monitor_thread, NULL, monitor, (void *)sim_coders) != 0)
	{
		free(sim_coders);
		free(monitor_thread);
		return (NULL);
	}
	return (monitor_thread);
}
