/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 22:09:23 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

/*
write lock to ensure only one thread can write to the terminal at once
*/
void	message(char *color, char *str, t_philo *philo)
{
	u_int64_t	time;

	pthread_mutex_lock(philo->write_lock);
	time = current_time() - philo->start_time;
	if (check_dead_flag(philo) == false)
		printf("%s%lu %d %s\n", color, time, philo->id, str);
	pthread_mutex_unlock(philo->write_lock);
}

/*
personal implementation of the standard strlen function
*/
size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

/*
personal implementation of the standard atoi function
*/
int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || (nptr[i] == 32))
		i++;
	if (nptr[i] == '-')
	{
		sign = sign * -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10;
		res = nptr[i] - 48 + res;
		i++;
	}
	return (res * sign);
}

// Destroys all the mutexes
int	free_and_destroy(int err_code, t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->meal_lock);
	pthread_mutex_destroy(&data->dead_lock);
	while (i < data->philos[0].n_philos)
	{
		pthread_mutex_destroy(data->forks[i]);
		free((data->forks[i]));
		i++;
	}
	free(data->forks);
	return (err_code);
}
