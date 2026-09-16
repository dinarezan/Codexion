/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/16 15:03:07 by drezan           ###   ########.fr       */
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
	printf("dongles initiaized.\n");
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

static void	*compile(void *sim_coder)
{
	t_coder			*coder;
	t_sim_param		*sim_param;
	long long		time_diff;

	coder = ((t_sim_coder *)sim_coder)->coder;
	sim_param = ((t_sim_coder *)sim_coder)->sim_param;

	pthread_mutex_lock(&((t_coder *)coder)->left->dongle);
	time_diff = time_difference(sim_param->sim_start);
	printf("%lld Programmer %d has taken a left dongle\n", time_diff,
		((t_coder *)coder)->id);
	pthread_mutex_lock(&((t_coder *)coder)->right->dongle);
	time_diff = time_difference(sim_param->sim_start);
	printf("%lld Programmer %d has taken a right dongle\n", time_diff,
		((t_coder *)coder)->id);
	time_diff = time_difference(sim_param->sim_start);
	printf("%lld Programmer %d is compiling\n", time_diff,
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_compile * 1000);
	pthread_mutex_unlock(&((t_coder *)coder)->left->dongle);
	pthread_mutex_unlock(&((t_coder *)coder)->right->dongle);
	return (NULL);
}

static void	*debug(void *sim_coder)
{
	t_coder			*coder;
	t_sim_param		*sim_param;
	long long		time_diff;

	coder = ((t_sim_coder *)sim_coder)->coder;
	sim_param = ((t_sim_coder *)sim_coder)->sim_param;

	time_diff = time_difference(sim_param->sim_start);
	printf("%lld Programmer %d is debugging.\n", time_diff, ((t_coder *)coder)->id);
	usleep(sim_param->time_to_debug * 1000);
	return (NULL);
}

static void	*refactor(void *sim_coder)
{
	t_coder			*coder;
	t_sim_param		*sim_param;
	long long		time_diff;

	coder = ((t_sim_coder *)sim_coder)->coder;
	sim_param = ((t_sim_coder *)sim_coder)->sim_param;

	time_diff = time_difference(sim_param->sim_start);
	printf("%lld Programmer %d is refactoring.\n", time_diff, ((t_coder *)coder)->id);
	usleep(sim_param->time_to_refactor * 1000);
	return (NULL);
}

void	coder_run(t_sim_param *sim_param, t_coder *coder)
{
	t_sim_coder	*sim_coder;
	
	sim_coder = malloc(sizeof(t_sim_coder));
	sim_coder->sim_param = sim_param;
	sim_coder->coder = coder;
	pthread_create(&coder->compile, NULL, compile, (void *)sim_coder);
	pthread_join(coder->compile, NULL);
	pthread_create(&coder->debug, NULL, debug, (void *)sim_coder);
	pthread_join(coder->debug, NULL);
	pthread_create(&coder->refactor, NULL, refactor, (void *)sim_coder);
	pthread_join(coder->refactor, NULL);
	free(sim_coder);
}
