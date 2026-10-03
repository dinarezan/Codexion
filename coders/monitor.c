/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 16:12:11 by drezan            #+#    #+#             */
/*   Updated: 2026/10/03 17:15:59 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

static int	monitor_helper(t_coder *coder, t_sim_param *sim_param)
{
	int				compile_count;
	struct timeval	last_compile;

	pthread_mutex_lock(&coder->state_lock);
	compile_count = coder->compile_count;
	last_compile = coder->last_compile;
	pthread_mutex_unlock(&coder->state_lock);
	if (compile_count < sim_param->number_of_compiles_required
		&& time_difference(last_compile) >= sim_param->time_to_burnout)
	{
		printf("%lld %d burned out\n", time_difference(sim_param->sim_start),
			coder->id);
		pthread_mutex_lock(&sim_param->stop_lock);
		sim_param->sim_stop = 1;
		pthread_mutex_unlock(&sim_param->stop_lock);
		return (1);
	}
	else
		return (0);
}

static void	broadcast_all(t_coder **coders)
{
	int	i;

	i = 0;
	while (coders[i])
	{
		pthread_mutex_lock(&coders[i]->left->dongle);
		pthread_cond_broadcast(&coders[i]->left->condition);
		pthread_mutex_unlock(&coders[i]->left->dongle);
		i++;
	}
}

void	*monitor(void *args)
{
	int			i;
	int			done;
	t_coder		**coders;
	t_sim_param	*sim_param;

	coders = ((t_sim_coders *)args)->coders;
	sim_param = ((t_sim_coders *)args)->sim_param;
	free(args);
	done = 0;
	while (done < sim_param->number_of_coders)
	{
		i = 0;
		done = 0;
		while (coders[i])
		{
			if (monitor_helper(coders[i], sim_param))
				return (broadcast_all(coders), NULL);
			pthread_mutex_lock(&coders[i]->state_lock);
			if (coders[i]->compile_count
				>= sim_param->number_of_compiles_required)
				done++;
			pthread_mutex_unlock(&coders[i++]->state_lock);
		}
	}
	return (NULL);
}

pthread_t	*init_monitor(t_coder **coders, t_sim_param *sim_param)
{
	pthread_t		*monitor_thread;
	t_sim_coders	*sim_coders;

	sim_coders = malloc(sizeof(t_sim_coders));
	if (!sim_coders)
		return (NULL);
	sim_coders->coders = coders;
	sim_coders->sim_param = sim_param;
	monitor_thread = malloc(sizeof(pthread_t));
	if (!monitor_thread)
		return (free(sim_coders), NULL);
	if (pthread_create(monitor_thread, NULL, monitor, (void *)sim_coders) != 0)
	{
		free(sim_coders);
		free(monitor_thread);
		return (NULL);
	}
	return (monitor_thread);
}
