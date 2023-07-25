/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 09:40:21 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/25 09:40:23 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

/* initializing the philo struct; id is incremented by +1 so that 
the id of the first philo is 1*/
int	init_philo_struct(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philo[i].data = data;
		data->philo[i].id = i + 1;
		data->philo[i].death_time = data->time_to_die; //! necessary, why not difference?
		data->philo[i].n_eat_times = 0;
		data->philo[i].eating = 0;
		data->philo[i].status = 0;
		pthread_mutex_init(&data->philo[i].lock, NULL); //! MUTEXES PHILO->LOCK
		i++; //! Why acces via data-> and not directly via philo struct?!
	}
	return (0);
}

/*sets the left fork of the first philosopher (data->philo[0]) 
to point to the first fork in the data->forks array;
sets the right fork of the first philosopher to point to the 
last fork in the data->forks array. This establishes a circular 
arrangement since the last fork is adjacent to the first fork
each philosopher is connected to its left and right neighbor, 
and the last philosopher is connected back to the first one
ex: philo 1 left fork is the right fork of philo 2;
&data->forks[0] gets the address of the first mutex in the forks
array; by using pointers only references of the actual mutexes are
passed on rather than making copies of the mutexes*/
int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philos)
		pthread_mutex_init(&data->forks[i], NULL); //! MUTEXES FOR EACH FORK
	i = 0;
	data->philo[0].fork_l = &data->forks[0];
	data->philo[0].fork_r = &data->forks[data->n_philos - 1];
	i = 1;
	while (i < data->n_philos)
	{
		data->philo[i].fork_l = &data->forks[i];
		data->philo[i].fork_r = &data->forks[i - 1];
		i++;
	}
	return (0);
}

/* initializing the data struct, while allocating memory for the pointer
elements of the struct (id, forks, philo);
initializing mutexes for write & lock struct elements */
int init_data_struct(t_data *data, int ac, char **av)
{
	data->n_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->n_meals = ft_atoi(av[5]);
	else
		data->n_meals = -1;
	data->start_time = get_time();
	data->dead = 0;
	data->finished = 0;
	data->id = malloc(sizeof(pthread_t) * data->n_philos);
	if (!data->id)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
	if (!data->forks)
		return (1);
	data->philo = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philo)
		return (1);
	pthread_mutex_init(&data->write, NULL); //! MUTEX DATA->WRITE
	pthread_mutex_init(&data->lock, NULL); //! MUTEX DATA->LOCK
	return (0);
}

int init_structs(t_data *data, int ac, char **av)
{
	if (init_data_struct(data, ac, av))
		return (1);
	if (init_forks(data))
		return (1);
	init_philo_struct(data);
	return(0);
}