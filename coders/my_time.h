/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_time.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 12:23:37 by drezan            #+#    #+#             */
/*   Updated: 2026/09/25 15:14:51 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_TIME_H

# define MY_TIME_H

# include <sys/time.h>

long long	time_difference(struct timeval start);

#endif
