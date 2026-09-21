/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/21 16:42:37 by drezan           ###   ########.fr       */
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

void	acquire_both_dongles(t_coder *c, t_sim_param *param)
{
	t_dongle	*d1;
	t_dongle	*d2;
	t_dongle	*first;
	t_dongle	*second;
	int			ticket1;
	int			ticket2;
	long long	cooldown1;
	long long	cooldown2;
	int			turn_ready;
	long long	rem1;
	long long	rem2;
	long long	max_rem;

	d1 = c->left;
	d2 = c->right;
	first = (d1->id < d2->id) ? d1 : d2;
	second = (d1->id < d2->id) ? d2 : d1;
	ticket1 = d1->ticket_counter++;
	ticket2 = d2->ticket_counter++;
	pthread_mutex_lock(&first->dongle);
	pthread_mutex_lock(&second->dongle);
	while (1)
	{
		turn_ready = (ticket1 == d1->current_turn)
			&& (ticket2 == d2->current_turn);
		if (turn_ready)
		{
			cooldown1 = time_difference(d1->last_compile);
			cooldown2 = time_difference(d2->last_compile);
			rem1 = param->dongle_cooldown - cooldown1;
			rem2 = param->dongle_cooldown - cooldown2;
			max_rem = (rem1 > rem2) ? rem1 : rem2;
			if (max_rem > 0)
			{
				pthread_mutex_unlock(&first->dongle);
				pthread_mutex_unlock(&second->dongle);
				usleep(max_rem * 1000);
				continue ;
			}
			pthread_mutex_lock(&first->dongle);
			pthread_mutex_lock(&second->dongle);
			break ;
		}
		pthread_mutex_unlock(&second->dongle);
		pthread_cond_wait(&first->condition, &first->dongle);
		pthread_mutex_lock(&second->dongle);
	}
	printf("%lld %d has taken a dongle\n", time_difference(param->sim_start),
		((t_coder *)c)->id);
	printf("%lld %d has taken a dongle\n", time_difference(param->sim_start),
		((t_coder *)c)->id);
}

void	release_both_dongles(t_coder *c)
{
	t_dongle	*d1;
	t_dongle	*d2;
	t_dongle	*first;
	t_dongle	*second;

	d1 = c->left;
	d2 = c->right;
	first = (d1->id < d2->id) ? d1 : d2;
	second = (d1->id < d2->id) ? d2 : d1;
	gettimeofday(&first->last_compile, NULL);
	gettimeofday(&second->last_compile, NULL);
	first->current_turn++;
	second->current_turn++;
	pthread_cond_broadcast(&first->condition);
	pthread_cond_broadcast(&second->condition);
	pthread_mutex_unlock(&first->dongle);
	pthread_mutex_unlock(&second->dongle);
}

void	*coder_run(void *sim_coder)
{
	t_coder		*coder;
	t_sim_param	*sim_param;

	coder = ((t_sim_coder *)sim_coder)->coder;
	sim_param = ((t_sim_coder *)sim_coder)->sim_param;
	acquire_both_dongles(coder, sim_param);
	printf("%lld %d is compiling.\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_compile * 1000);
	release_both_dongles(coder);
	printf("%lld %d is debugging.\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_debug * 1000);
	printf("%lld %d is refactoring.\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_refactor * 1000);
	return (NULL);
}
