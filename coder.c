/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/14 16:51:17 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

t_coder	**init_coders(int n)
{
	t_coder	**coders;

	coders = (t_coder **)malloc(n * sizeof(t_coder *));
	for (int i = 0; i < n; i++)
	{
		coders[i] = (t_coder *)malloc(sizeof(t_coder));
		coders[i]->id = i + 1;
		coders[i]->compile_count = 0;
		coders[i]->last_compile = 0;
	}
	return (coders);
}

static void	*compile(void *dongle, int id)
{
	pthread_mutex_lock((pthread_mutex_t *)dongle);
	printf("Programmer %d is compiling\n", id);
	usleep(900000);
	printf("Programmer %d is done compiling\n", id);
	pthread_mutex_unlock((pthread_mutex_t *)dongle);
}

static void	*debug(int id)
{
	printf("Programmer %d is debugging", id);
	usleep(500000);
}

static void	*refactor(int id)
{
	printf("Programmer %d is refactoring", id);
	usleep(500000);
}

void	coder_run(t_coder *coder, t_dongle *dongle)
{
	pthread_create(&coder->coder, NULL, compile, dongle);
	pthread_create(&coder, NULL, debug, NULL);
	pthread_create(&coder, NULL, refactor, NULL);
	pthread_join(coder->coder, NULL);
}
