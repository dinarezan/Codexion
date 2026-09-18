/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/18 15:30:46 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "dongle.h"

t_coder	**init_coders(t_sim_param *sim_param)
{
	t_coder		**coders;
	t_dongle	**dongles;
	int			n;

	n = sim_param->number_of_coders;
	dongles = init_dongles(sim_param);
	printf("Dongles initialized.\n");
	coders = (t_coder **)malloc(n * sizeof(t_coder *));
	for (int i = 0; i < n; i++)
	{
		coders[i] = (t_coder *)malloc(sizeof(t_coder));
		coders[i]->id = i + 1;
		coders[i]->compile_count = 0;
		coders[i]->last_compile = sim_param->sim_start;
		if (i == 0)
			coders[i]->left = dongles[n - 1];
		else
			coders[i]->left = dongles[i - 1];
		coders[i]->right = dongles[i];
	}
	return (coders);
}

void	acquire_single_dongle(t_dongle *d, t_sim_param *param)
{
	int	my_ticket;
	int	elapsed_cooldown;
	int	remaining_us;

	pthread_mutex_lock(&d->dongle);
	my_ticket = d->ticket_counter++;
	while (1)
	{
		elapsed_cooldown = time_difference(d->last_compile);
		if (my_ticket == d->current_turn
			&& elapsed_cooldown >= param->dongle_cooldown)
			break ;
		if (my_ticket == d->current_turn
			&& elapsed_cooldown < param->dongle_cooldown)
		{
			remaining_us = (param->dongle_cooldown - elapsed_cooldown) * 1000;
			pthread_mutex_unlock(&d->dongle);
			usleep(remaining_us);
			pthread_mutex_lock(&d->dongle);
			continue ;
		}
		pthread_cond_wait(&d->condition, &d->dongle);
	}
}

void	release_single_dongle(t_dongle *d)
{
	d->current_turn++;
	gettimeofday(&d->last_compile, NULL);
	pthread_cond_broadcast(&d->condition);
	pthread_mutex_unlock(&d->dongle);
}

void	*coder_run(void *sim_coder)
{
	t_coder		*coder;
	t_sim_param	*sim_param;
	t_dongle	*first;
	t_dongle	*second;

	coder = ((t_sim_coder *)sim_coder)->coder;
	sim_param = ((t_sim_coder *)sim_coder)->sim_param;
	if (coder->left->id < coder->right->id)
	{
		first = coder->left;
		second = coder->right;
	}
	else
	{
		first = coder->right;
		second = coder->left;
	}
	acquire_single_dongle(first, sim_param);
	printf("%lld %d has taken a dongle\n",
		time_difference(sim_param->sim_start), ((t_coder *)coder)->id);
	acquire_single_dongle(second, sim_param);
	printf("%lld %d has taken a dongle\n",
		time_difference(sim_param->sim_start), ((t_coder *)coder)->id);
	printf("%lld %d is compiling\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_compile * 1000);
	release_single_dongle(second);
	release_single_dongle(first);
	printf("%lld %d is debugging.\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_debug * 1000);
	printf("%lld %d is refactoring.\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_refactor * 1000);
	return (NULL);
}
