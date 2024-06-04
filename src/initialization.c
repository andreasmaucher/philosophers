/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:13:39 by amaucher          #+#    #+#             */
/*   Updated: 2024/06/04 22:11:18 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	init_philos_struct_basics(t_philo *philos, char **av, t_data *data, int i)
{
	philos[i].id = i + 1;
	philos[i].eating = false;
	philos[i].time_to_die = ft_atoi(av[2]);
	philos[i].time_to_eat = ft_atoi(av[3]);
	philos[i].time_to_sleep = ft_atoi(av[4]);
	philos->n_philos = data->n_philos;
	if (av[5])
		philos[i].num_times_to_eat = ft_atoi(av[5]);
	else
		philos[i].num_times_to_eat = -1;
	philos[i].meals_counter = 0;
	philos->finished = 0;
	philos[i].last_meal = current_time();
	philos[i].write_lock = &data->write_lock;
	philos[i].dead_lock = &data->dead_lock;
	philos[i].dead = &data->dead_flag;
	philos[i].meal_lock = &data->meal_lock;
	philos[i].start_time = current_time();
}

/*
sets the left fork of the first philosopher (data->philo[0]) 
to point to the first fork in the data->forks array;
sets the right fork of the first philosopher to point to the 
last fork in the data->forks array. This establishes a circular 
arrangement since the last fork is adjacent to the first fork
each philosopher is connected to its left and right neighbor, 
and the last philosopher is connected back to the first one
ex: philo 1 left fork is the right fork of philo 2;
*/
void	init_philos_struct(t_philo *philos, t_data *data, char **av)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		init_philos_struct_basics(philos, av, data, i);
		if (i == 0)
		{
			philos[i].l_fork = data->forks[i];
			philos[i].r_fork = data->forks[data->n_philos - 1];
		}
		else
		{
			philos[i].l_fork = data->forks[i];
			philos[i].r_fork = data->forks[i - 1];
		}
		i++;
	}
}

/*
the forks array does not need to be null-terminated, because it is an array of pointers to
pthread_mutex_t and not an array of strings (null-terminated character array)
*/
void	init_data_struct(t_philo *philos, t_data *data, char **av)
{
	int	i;

	i = 0;
	data->dead_flag = false;
	data->philos = philos;
	data->n_philos = ft_atoi((av[1]));
	data->forks = malloc(sizeof(pthread_mutex_t *) * data->n_philos);
	while (i < data->n_philos)
	{
		data->forks[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->meal_lock, NULL);
	pthread_mutex_init(&data->dead_lock, NULL);
}

/*
initialization of all structs and fork mutexes;
*/
void	init_structs(t_philo *philos, t_data *data, char **av)
{
	init_data_struct(philos, data, av);
	init_philos_struct(philos, data, av);
}
