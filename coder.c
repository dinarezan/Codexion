/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 17:42:19 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "dongle.h"

t_coder	**init_coders(t_sim_param *sim_param, t_dongle **dongles)
{
	t_coder	**coders;
	int		i;
	int		n;

	i = 0;
	n = sim_param->number_of_coders;
	coders = (t_coder **)malloc((n + 1) * sizeof(t_coder *));
	if (!coders)
		return (free_dongles(dongles), NULL);
	while (i < n)
	{
		coders[i] = (t_coder *)malloc(sizeof(t_coder));
		if (!coders[i])
			return (free_dongles(dongles), free_coders(coders), NULL);
		coders[i]->id = i + 1;
		coders[i]->compile_count = 0;
		if (i == 0)
			coders[i]->left = dongles[n - 1];
		else
			coders[i]->left = dongles[i - 1];
		coders[i]->right = dongles[i];
		i++;
	}
	coders[i] = NULL;
	return (coders);
}

static void	compile(t_coder *coder, t_sim_param *sim_param)
{
	printf("%lld %d is compiling.\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_compile * 1000);
	release_both_dongles(coder);
}

static void	debug(t_coder *coder, t_sim_param *sim_param)
{
	printf("%lld %d is debugging.\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_debug * 1000);
}

static void	refactor(t_coder *coder, t_sim_param *sim_param)
{
	printf("%lld %d is refactoring.\n", time_difference(sim_param->sim_start),
		((t_coder *)coder)->id);
	usleep(sim_param->time_to_refactor * 1000);
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
		if (sim_stop(sim_param))
			break ;
		compile(coder, sim_param);
		if (sim_stop(sim_param))
			break ;
		debug(coder, sim_param);
		if (sim_stop(sim_param))
			break ;
		refactor(coder, sim_param);
		coder->compile_count++;
	}
	return (NULL);
}
