/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_validation.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drezan <drezan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 17:37:22 by drezan            #+#    #+#             */
/*   Updated: 2026/09/21 17:40:53 by drezan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>

static long long	ft_atoi_ll(char *nptr)
{
	long long	nbr;
	int			i;
	int			minus;

	minus = 1;
	nbr = 0;
	i = 0;
	if (nptr[i] == '-')
	{
		minus = -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nbr = nbr * 10 + (nptr[i] - '0');
		i++;
	}
	return (minus * nbr);
}

static int	ft_integer_check(char *nptr)
{
	long long	nbr;
	int			i;

	i = 0;
	if (!nptr)
		return (-1);
	if (*nptr != '0' && atoi(nptr) == 0)
		return (-1);
	while (nptr[i])
	{
		if (!(nptr[i] >= 48 && nptr[i] <= 57) && nptr[i] != '-'
			&& nptr[i] != '+')
			return (-1);
		i++;
	}
	nbr = ft_atoi_ll(nptr);
	if (nbr > INT_MAX || nbr < INT_MIN)
		return (-1);
	return (0);
}

int	arg_check(char **argv)
{
	int	i;

	i = 1;
	while (i < 8)
	{
		if (ft_integer_check(argv[i]) < 0 || atoi(argv[i]) < 0)
			return (-1);
		i++;
	}
	if (atoi(argv[1]) < 1)
		return (-1);
	return (0);
}
