/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 15:59:36 by drezan            #+#    #+#             */
/*   Updated: 2026/09/14 16:02:31 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIMULATION_H

#define SIMULATION_H

typedef struct s_sim_param
{
	int time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
}	t_sim_param;


#endif
