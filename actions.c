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

void	thinking(t_philo *philo)
{
	message(BLUE, THINKING, philo);
}

void	sleeping(t_philo *philo)
{
	message(GREY, SLEEPING, philo);
	ft_usleep(philo->time_to_sleep);
}

/*
mutexes for forks are locked, to ensure that one fork can only be used 
by one philo at a time;
in case of one single philo he can only pick up one fork and dies immediately, 
since eating is only possible with two forks;
*/
void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	message(YELLOW, TAKE_FORKS, philo);
	if (philo->n_philos == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->r_fork);
		return ;
	}
	pthread_mutex_lock(philo->l_fork);
	message(YELLOW, TAKE_FORKS, philo);
}

void	meals(t_philo *philos)
{
	pthread_mutex_lock(philos->meal_lock);
	philos->last_meal = current_time();
	philos->meals_counter++;
	pthread_mutex_unlock(philos->meal_lock);
}

void	eating(t_philo *philos)
{
	take_forks(philos);
	philos->eating = true;
	message(GREEN, EATING, philos);
	meals(philos);
	ft_usleep(philos->time_to_eat);
	philos->eating = false;
	pthread_mutex_unlock(philos->l_fork);
	pthread_mutex_unlock(philos->r_fork);
}
