/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:14:57 by drezan            #+#    #+#             */
/*   Updated: 2026/09/16 12:39:57 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H

# define DONGLE_H

# include "simulation.h"
# include <pthread.h>

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	dongle;
	pthread_cond_t	condition;
	struct timeval	last_compile;
}					t_dongle;

t_dongle			**init_dongles(t_sim_param *sim_param);

#endif