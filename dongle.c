/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 12:20:43 by drezan            #+#    #+#             */
/*   Updated: 2026/09/15 15:14:19 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"

t_dongle	**init_dongles(int n)
{
	t_dongle	**dongles;

	dongles = (t_dongle **)malloc(n * sizeof(t_dongle *));
	if (!dongles)
		return (NULL);
	for (int i = 0; i < n; i++)
	{
		dongles[i] = malloc(sizeof(t_dongle));
		if (!dongles[i])
			return (NULL);
		pthread_mutex_init(&dongles[i]->dongle, NULL);
		dongles[i]->id = i;
		dongles[i]->last_compile = 0;
	}
	return (dongles);
}
