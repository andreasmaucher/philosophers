/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 13:26:48 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/11 13:26:49 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

/* different order for odd or even n of philos */
void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_r);
	messages(YELLOW, TAKE_FORKS, philo);
	pthread_mutex_lock(philo->fork_l);
	messages(YELLOW, TAKE_FORKS, philo);
}

/* unlocking both forks and setting the philo to sleep */
void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->fork_l);
	pthread_mutex_unlock(philo->fork_r);
	messages(GREY, SLEEPING, philo);
	ft_usleep(philo->data->time_to_sleep);
}