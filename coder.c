/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:26:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/15 16:10:49 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include "dongle.h"
#include <sys/time.h>

t_coder	**init_coders(int n)
{
	t_coder		**coders;
	t_dongle	**dongles;

	dongles = init_dongles(n);
	printf("dongles initiaized.\n");
	coders = (t_coder **)malloc(n * sizeof(t_coder *));
	for (int i = 0; i < n; i++)
	{
		coders[i] = (t_coder *)malloc(sizeof(t_coder));
		coders[i]->id = i + 1;
		coders[i]->compile_count = 0;
		coders[i]->last_compile = 0;
		if (i == 0)
			coders[i]->left = dongles[n - 1];
		else
			coders[i]->left = dongles[i - 1];
		coders[i]->right = dongles[i];
	}
	return (coders);
}

static void	*compile(void *coder)
{
	struct timeval	start;
	struct timeval	end;
	int				rc;

	rc = gettimeofday(&start, NULL);
	if (rc < 0)
		return (printf("failed...\n"), NULL);
	pthread_mutex_lock(&((t_coder *)coder)->left->dongle);
	pthread_mutex_lock(&((t_coder *)coder)->right->dongle);
	printf("%ld.%06ld Programmer %d is compiling\n", start.tv_sec,
		start.tv_usec, ((t_coder *)coder)->id);
	usleep(900000);
	gettimeofday(&end, NULL);
	printf("%ld.%06ld Programmer %d is done compiling.\n", end.tv_sec,
		end.tv_usec, ((t_coder *)coder)->id);
	pthread_mutex_unlock(&((t_coder *)coder)->left->dongle);
	pthread_mutex_unlock(&((t_coder *)coder)->right->dongle);
	return (NULL);
}

static void	*debug(void *coder)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	printf("%ld.%06ld Programmer %d is debugging.\n", start.tv_sec,
		start.tv_usec, ((t_coder *)coder)->id);
	usleep(500000);
	return (NULL);
}

static void	*refactor(void *coder)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	printf("%ld.%06ld Programmer %d is refactoring.\n", start.tv_sec,
		start.tv_usec, ((t_coder *)coder)->id);
	usleep(500000);
	return (NULL);
}

void	coder_run(t_coder *coder)
{
	pthread_create(&coder->compile, NULL, compile, (void *)coder);
	pthread_join(coder->compile, NULL);
	pthread_create(&coder->debug, NULL, debug, (void *)coder);
	pthread_join(coder->debug, NULL);
	pthread_create(&coder->refactor, NULL, refactor, (void *)coder);
	pthread_join(coder->refactor, NULL);
}
