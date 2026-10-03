/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 14:30:12 by drezan            #+#    #+#             */
/*   Updated: 2026/10/03 20:20:41 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "dongle.h"

static int	wait_for_cooldown(t_dongle *d, t_sim_param *param)
{
	long long		deadline;
	long long		now;
	struct timespec	ts;
	struct timeval	tv;

	deadline = d->last_compile.tv_sec * 1000
		+ d->last_compile.tv_usec / 1000 + param->dongle_cooldown;
	gettimeofday(&tv, NULL);
	now = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (now >= deadline)
		return (0);
	ts.tv_sec = deadline / 1000;
	ts.tv_nsec = (deadline % 1000) * 1000000;
	pthread_cond_timedwait(&d->condition, &d->dongle, &ts);
	return (1);
}

static void	acquire_single_dongle(t_dongle *d, t_sim_param *param, t_coder *c)
{
	pthread_mutex_lock(&d->dongle);
	insert_heap(d->queue, c);
	while (!sim_stop(param))
	{
		if (d->queue->coders[0]->id == c->id && !d->in_use)
		{
			if (!wait_for_cooldown(d, param))
				break ;
		}
		else
			pthread_cond_wait(&d->condition, &d->dongle);
	}
	if (sim_stop(param))
	{
		pthread_mutex_unlock(&d->dongle);
		return ;
	}
	pop_min_from_heap(d->queue);
	d->in_use = 1;
	pthread_mutex_unlock(&d->dongle);
}

static void	release_single_dongle(t_dongle *d)
{
	pthread_mutex_lock(&d->dongle);
	gettimeofday(&d->last_compile, NULL);
	d->in_use = 0;
	pthread_cond_broadcast(&d->condition);
	pthread_mutex_unlock(&d->dongle);
}

void	acquire_both_dongles(t_coder *c, t_sim_param *param)
{
	t_dongle	*first;
	t_dongle	*second;

	if (c->left->id < c->right->id)
	{
		first = c->left;
		second = c->right;
	}
	else
	{
		first = c->right;
		second = c->left;
	}
	acquire_single_dongle(first, param, c);
	if (sim_stop(param))
		return ;
	printf("%lld %d has taken a dongle\n", time_difference(param->sim_start),
		c->id);
	acquire_single_dongle(second, param, c);
	if (sim_stop(param))
		return ;
	printf("%lld %d has taken a dongle\n", time_difference(param->sim_start),
		c->id);
}

void	release_both_dongles(t_coder *c)
{
	release_single_dongle(c->left);
	release_single_dongle(c->right);
}
