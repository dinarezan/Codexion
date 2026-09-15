/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 16:37:15 by drezan            #+#    #+#             */
/*   Updated: 2026/09/14 16:44:01 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dongle.h"
#include "coder.h"
#include "simulation.h"

int main()
{
	t_coder **coders = init_coders(10);
	t_dongle *dongle;
	
	pthread_mutex_init(dongle->dongle, NULL);
	
	for (int i = 0; i < 10; i++)
	{
		coder_run(coders[i], dongle->dongle);
	}
	pthread_mutex_destroy(dongle->dongle);
	return 0;
}
