/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:13:42 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//DONE
/*
sets the data into sleep mode for the inputted amount of ms or in other words
until the time elapsed (current_time - start time) is less than the sleep time;
500 in this case stands for 500 microseconds or 0.5ms;
*/
int	ft_usleep(u_int64_t time_ms)
{
	u_int64_t	start;

	start = current_time();
	while ((current_time() - start) < time_ms)
		usleep(500); //! ADJUST?!
	return (0);
}

//DONE
//! maybe in size_t?! but uint is my own
/* tv stands for timeval and returns current time; returns time 
in milliseconds*/
u_int64_t	current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (printf("gettimeofday() FAILURE\n"));
	return ((tv.tv_sec * (u_int64_t)1000) + (tv.tv_usec / 1000));
}