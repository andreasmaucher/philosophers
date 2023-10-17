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

/*
function locks the philo->dead element to ensure no other threads write
to it simultaneously; after checking the value the mutex is unlocked again
and the corresponding boolean value is returned
*/
bool	check_dead_flag(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*philo->dead == true)
	{
		pthread_mutex_unlock(philo->dead_lock);
		return (true);
	}
	pthread_mutex_unlock(philo->dead_lock);
	return (false);
}

/*
routine that lets the philosophers eat, sleep & think;
philo_pointer expects a pointer to a philo struct to identify each 
distinct philosopher;
*/
void	*routine(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *)philo_pointer;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (check_dead_flag(philo) == false)
	{
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (philo_pointer);
}

/*
creates one thread for each philosopher and a separate supervisor thread;
the first argument is to store the id of the thread & the last argumentis a pointer to the data 
that gets passed; after the routine is finished the threads are joined;
*/
int	create_threads(t_data *data)
{
	int			i;
	pthread_t	supervisor_thread;

	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, &routine, &data->philos[i]))
			free_and_destroy(THREAD_ERR, data);
		i++;
	}
	if (pthread_create(&supervisor_thread, NULL, &supervisor, data->philos))
		free_and_destroy(THREAD_ERR, data);
	if (pthread_join(supervisor_thread, NULL))
		free_and_destroy(THREAD_ERR, data);
	i = 0;
	while (i < data->n_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL))
			free_and_destroy(THREAD_ERR, data);
		i++;
	}
	return (0);
}
