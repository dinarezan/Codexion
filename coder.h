/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 12:06:48 by drezan            #+#    #+#             */
/*   Updated: 2026/09/14 16:45:51 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H

# define CODER_H

# include "dongle.h"
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_coder
{
	int			id;
	pthread_t	coder;
	t_dongle	*left;
	t_dongle	*right;
	int			compile_count;
	long		last_compile;
}				t_coder;

t_coder			**init_coders(int n);
void			coder_run(t_coder *coder, t_dongle *dongle);

#endif