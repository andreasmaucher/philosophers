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

/* tv stands for timeval and returns current time; returns time 
in milliseconds*/
t_ms	get_time(void)
{
	struct timeval	tv;
	t_ms 			time_in_ms;

	if (gettimeofday(&tv, NULL))
		return (0);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

/* don't fully understands why this works yet */
int	ft_usleep(t_ms time)
{
	t_ms	start;

	start = get_time();
	while ((get_time() - start) < time)
		usleep(1000);
	return (0);
}