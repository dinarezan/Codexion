/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:59:36 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 12:06:42 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H

# define SIMULATION_H

# include "my_time.h"
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_coder	t_coder;

typedef struct s_sim_param
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					number_of_compiles_required;
	int					dongle_cooldown;
	int					scheduler;
	struct timeval		sim_start;
	int					sim_stop;
	pthread_mutex_t		stop_lock;
}						t_sim_param;

typedef struct s_sim_coder
{
	t_sim_param			*sim_param;
	t_coder				*coder;
}						t_sim_coder;

typedef struct s_sim_coders
{
	t_sim_param			*sim_param;
	t_coder				**coders;
}						t_sim_coders;

#endif
