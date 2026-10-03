/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_time.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 12:18:17 by drezan            #+#    #+#             */
/*   Updated: 2026/09/16 14:56:00 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/time.h>

long long	time_difference(struct timeval start)
{
	struct timeval	end;
	long long		start_ms;
	long long		end_ms;
	long long		diff_ms_int;

	gettimeofday(&end, NULL);
	start_ms = ((long long)start.tv_sec * 1000) + (start.tv_usec / 1000);
	end_ms = ((long long)end.tv_sec * 1000) + (end.tv_usec / 1000);
	diff_ms_int = end_ms - start_ms;
	return (diff_ms_int);
}
