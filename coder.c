/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/24 12:47:00 by drezan           ###   ########.fr       */
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
	int				ticket;
	long long		cooldown_over;
	struct timeval	now;
	struct timespec	ts;

	pthread_mutex_lock(&d->dongle);
	ticket = d->ticket_counter++;
	while (ticket != d->current_turn)
	{
		pthread_cond_wait(&d->condition, &d->dongle);
	}
	cooldown_over = d->last_compile.tv_sec * 1000 + d->last_compile.tv_usec
		/ 1000 + param->dongle_cooldown;
	ts.tv_sec = cooldown_over / 1000;
	ts.tv_nsec = (cooldown_over % 1000) * 1000000;
	gettimeofday(&now, NULL);
	while (((long long)now.tv_sec * 1000 + now.tv_usec / 1000) < cooldown_over)
	{
		pthread_cond_timedwait(&d->condition, &d->dongle, &ts);
		gettimeofday(&now, NULL);
	}
	pthread_mutex_unlock(&d->dongle);
}

void acquire_both_dongles(t_coder *c, t_sim_param *param)
{
	t_dongle *first;
	t_dongle *second;
	
	first = (c->left->id < c->right->id) ? c->left : c->right;
	second = (c->left->id < c->right->id) ? c->right : c->left;
	acquire_single_dongle(first, param);
	printf("%lld %d has taken a dongle\n", time_difference(param->sim_start),
		((t_coder *)c)->id);
	acquire_single_dongle(second, param);
	printf("%lld %d has taken a dongle\n", time_difference(param->sim_start),
		((t_coder *)c)->id);
}

void	release_single_dongle(t_dongle *d)
{
	pthread_mutex_lock(&d->dongle);
	gettimeofday(&d->last_compile, NULL);
	d->current_turn++;
	pthread_cond_broadcast(&d->condition);
	pthread_mutex_unlock(&d->dongle);
}

void	release_both_dongles(t_coder *c)
{
	release_single_dongle(c->left);
	release_single_dongle(c->right);
}

void	*coder_run(void *sim_coder)
{
	t_coder		*coder;
	t_sim_param	*sim_param;

	coder = ((t_sim_coder *)sim_coder)->coder;
	sim_param = ((t_sim_coder *)sim_coder)->sim_param;
	while (coder->compile_count < sim_param->number_of_compiles_required)
	{
		acquire_both_dongles(coder, sim_param);
		gettimeofday(&coder->last_compile, NULL);
		printf("%lld %d is compiling.\n", time_difference(sim_param->sim_start),
			((t_coder *)coder)->id);
		usleep(sim_param->time_to_compile * 1000);
		release_both_dongles(coder);
		printf("%lld %d is debugging.\n", time_difference(sim_param->sim_start),
			((t_coder *)coder)->id);
		usleep(sim_param->time_to_debug * 1000);
		printf("%lld %d is refactoring.\n",
			time_difference(sim_param->sim_start), ((t_coder *)coder)->id);
		usleep(sim_param->time_to_refactor * 1000);
		coder->compile_count++;
	}
	return (NULL);
}
