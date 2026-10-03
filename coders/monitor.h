/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/24 16:48:13 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 15:14:36 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITOR_H

# define MONITOR_H

# include "coder.h"

pthread_t	*init_monitor(t_coder **coders, t_sim_param *sim_param);

#endif