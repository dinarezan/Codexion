/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 14:30:12 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 17:41:07 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "dongle.h"

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

static void	acquire_single_dongle(t_dongle *d, t_sim_param *param)
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

static void	release_single_dongle(t_dongle *d)
{
	pthread_mutex_lock(&d->dongle);
	gettimeofday(&d->last_compile, NULL);
	d->current_turn++;
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
		first = c->left;
		second = c->right;
	}
	acquire_single_dongle(first, param);
	if (sim_stop(param))
		return ;
	printf("%lld %d has taken a dongle\n", time_difference(param->sim_start),
		((t_coder *)c)->id);
	acquire_single_dongle(second, param);
	if (sim_stop(param))
		return ;
	printf("%lld %d has taken a dongle\n", time_difference(param->sim_start),
		((t_coder *)c)->id);
}

void	release_both_dongles(t_coder *c)
{
	release_single_dongle(c->left);
	release_single_dongle(c->right);
}
