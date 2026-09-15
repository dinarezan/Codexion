/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:14:57 by drezan            #+#    #+#             */
/*   Updated: 2026/09/14 16:35:58 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DONGLE_H

#define DONGLE_H

#include <pthread.h>

typedef struct s_dongle
{
	pthread_mutex_t	*dongle;
	pthread_cond_t	condition;
	int				last_compile;
}	t_dongle;


#endif