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

*/
bool	check_if_dead(t_philo *philo, size_t time_to_die)
{
	pthread_mutex_lock(philo->meal_lock);
	if (current_time() - philo->last_meal >= time_to_die && philo->eating == false)
	{
		pthread_mutex_unlock(philo->meal_lock);
		return (true);
	}
	pthread_mutex_unlock(philo->meal_lock);
	return (false);
}

int	count_finished_philos(t_philo *philos)
{
	int	i;
	int finished;

	i = 0;
	finished = 0;
	while (i < philos->n_philos)
	{
		pthread_mutex_lock(philos[i].meal_lock);
		if (philos[i].meals_counter >= philos[i].num_times_to_eat)
			finished++;
		pthread_mutex_unlock(philos[i].meal_lock);
		i++;
	}
	return(finished);
}

void	send_death_message(t_philo *philos)
{
	message(RED, DIED, philos);
}

//! this means we are using the dead flag as termination and dont necessarily write a death message
//! no death message in case of meals eaten reached
void	set_dead_flag(t_philo *philos)
{
	pthread_mutex_lock(philos[0].dead_lock);
	*philos->dead = true;
	pthread_mutex_unlock(philos[0].dead_lock);
}

//! does it have to be philos[0].dead_lock?
/*
supervisor thread that checks if a philosopher died or if they ate the required
number of meals (if specified in input);
philo_pointer expects a pointer to a philo struct to identify each 
distinct philosopher;
*/
void	*supervisor(void *philo_pointer)
{
	t_philo	*philos;
	int	i;

	philos = (t_philo *)philo_pointer;
	while (1)
	{
		if (philos->num_times_to_eat != -1 && count_finished_philos(philos) == philos->n_philos)
		{
			set_dead_flag(philos);
			return (philo_pointer);
		}
		i = 0;
		while (i < philos->n_philos)
		{
			if (check_if_dead(&philos[i], philos[i].time_to_die) == true)
			{
				send_death_message(philos);
				set_dead_flag(philos);
				return (philo_pointer);
			}
			i++;
		}
	}
}
