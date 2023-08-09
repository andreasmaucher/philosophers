/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 10:57:24 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/21 10:57:26 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

/* 
1. setting the start time
2. creating threads for each philosopher while executing the routine function
(if n_meals parameter is set a meals_monitor thread is created o check when all philos have
eaten n_meals); the first argument is to store the id of the thread & the last argument
is a pointer to the data that gets passed
3. after the routine is finished the threads are joined;
*/
int	handle_threads(t_data *data)
{
	int			i;
	pthread_t	meals_monitor_thread;

	data->start_time = get_time();
	if (data->n_meals > 0)
	{
		if (pthread_create(&meals_monitor_thread, NULL, &meals_monitor, &data->philo[0]))
			return (THREAD_ERR);
	}
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_create(&data->id[i], NULL, &routine, &data->philo[i]))
			return (THREAD_ERR);
		i++;
	}
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_join(data->id[i], NULL))
			return (THREAD_JOIN_ERR);
		i++;
	}
	return (0);
}