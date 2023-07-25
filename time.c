/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 13:39:13 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/20 13:39:15 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

// struct timeval {
//     time_t      tv_sec;     // seconds
//     suseconds_t tv_usec;    // microseconds
// };

// int gettimeofday(struct timeval *tv, struct timezone *tz);

/* tv stands for timeval and returns current time; returns time in milliseconds*/
t_ms	get_time(void)
{
	struct timeval	tv;
	t_ms 		time_in_ms;

	if (gettimeofday(&tv, NULL))
		return (0);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}