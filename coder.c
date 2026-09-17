/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/17 15:57:54 by drezan           ###   ########.fr       */
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

void	*coder_run(void *sim_coder)
{
	t_coder		*coder;
	t_sim_param	*sim_param;
	long long	time_diff;

	coder = ((t_sim_coder *)sim_coder)->coder;
	sim_param = ((t_sim_coder *)sim_coder)->sim_param;

	pthread_mutex_lock(&((t_coder *)coder)->left->dongle);
	time_diff = time_difference(sim_param->sim_start);
	printf("%lld %d has taken a left dongle\n", time_diff,
		((t_coder *)coder)->id);
	pthread_mutex_lock(&((t_coder *)coder)->right->dongle);
	time_diff = time_difference(sim_param->sim_start);
	printf("%lld %d has taken a right dongle\n", time_diff,
		((t_coder *)coder)->id);
	time_diff = time_difference(sim_param->sim_start);
	printf("%lld %d is compiling\n", time_diff,
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_compile * 1000);
	pthread_mutex_unlock(&((t_coder *)coder)->left->dongle);
	pthread_mutex_unlock(&((t_coder *)coder)->right->dongle);
	time_diff = time_difference(sim_param->sim_start);
	printf("%lld %d is debugging.\n", time_diff,
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_debug * 1000);
	time_diff = time_difference(sim_param->sim_start);
	printf("%lld %d is refactoring.\n", time_diff,
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_refactor * 1000);
	return (NULL);
}
