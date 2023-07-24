/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 11:40:01 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/20 11:40:03 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"
/* 
number_of_datas (same number as forks)
time_to_die
time_to_eat
time_to_sleep
number of times each data must eat (optional)
*/

pthread_mutex_t mutex;
int mails = 0;

void* routine() {
    for (int i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

long int	get_time(void)
{
	struct timeval	tv;
	long int 		time_in_ms;

	if (gettimeofday(&tv, NULL))
		return (0);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

pthread_mutex_t	*create_forks(t_data *data)
{
	int	i;
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
	if (forks == NULL)
		return (0);
	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_init(&forks[i], NULL);
		printf("Fork %d has been created\n", i);
		i++;
	}
	return (forks);
}

int	join_threads(t_data *data)
{
	int i;

	i = 0;
	while (i < data->n_philos)
	{
        if (pthread_join(data->philo_threads[i], NULL) != 0)
            return (1);
        printf("Thread %d has finished execution\n", i);
		i++;
    }
	return (0);
}

/* also fill philo struct */
int create_philos(t_data *data)
{
	int i;

	data->philo_threads = malloc(sizeof(pthread_t) * data->n_philos);
	if (data->philo_threads == NULL)
		return (0);
	i = 0;
	while (i < data->n_philos)
	{
        if (pthread_create(&data->philo_threads[i], NULL, &routine, NULL) != 0)
            return 1;
        printf("Thread %d has started\n", i);
		i++;
    }
	printf("Number of mails: %d\n", mails);
    return (0);
}

void	*monitor(void *data_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) data_pointer;
	pthread_mutex_lock(&philo->data->write);
	printf("data val: %d", philo->data->dead);
	pthread_mutex_unlock(&philo->data->write);
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->data->finished >= philo->data->n_philos)
			philo->data->dead = 1;
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}



/* initializing & creating threads; if n_meals parameter is set monitor
thread is created o check when all philos have eaten n_meals*/
int	thread_init(t_data *data)
{
	int			i;
	pthread_t	t0;

	i = -1;
	data->start_time = get_time();
	if (data->n_meals > 0)
	{
		if (pthread_create(&t0, NULL, &monitor, &data->philo[0]))
			return (error(TH_ERR, data));
	}
	while (++i < data->n_philos)
	{
		if (pthread_create(&data->id[i], NULL, &routine, &data->philo[i]))
			return (error(TH_ERR, data));
		ft_usleep(1);
	}
	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_join(data->id[i], NULL))
			return (error(JOIN_ERR, data));
	}
	return (0);
}

int	init_philo_struct(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philo[i].data = data;
		data->philo[i].id = i + 1;
		data->philo[i].time_to_die2 = data->time_to_die;
		data->philo[i].n_eat_times = 0;
		data->philo[i].eating = 0;
		data->philo[i].status = 0;
		pthread_mutex_init(&data->philo[i].lock, NULL);
		i++;
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
ex: philo 1 left fork is the right fork of philo 2*/
int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_philos)
		pthread_mutex_init(&data->forks[i], NULL);
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

//! change to ft_atoi
int init_data_struct(t_data *data, int ac, char **av)
{
	data->id = malloc(sizeof(pthread_t) * data->n_philos);
	if (!data->id)
		return (1);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
	if (!data->forks)
		return (1);
	data->philo = malloc(sizeof(t_philo) * data->n_philos);
	if (!data->philo)
		return (1);
	data->n_philos = atoi(av[1]);
	data->time_to_die = atoi(av[2]);
	data->time_to_eat = atoi(av[3]);
	data->time_to_sleep = atoi(av[4]);
	if (ac == 6)
		data->n_meals = atoi(av[5]);
	else
		data->n_meals = -1;
	data->start_time = get_time();
	data->dead = 0;
	data->finished = 0;
	pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->lock, NULL);
	/* create_philos(data);
	create_forks(data);
	join_threads(data); */
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

int	main(int ac, char *av[])
{
	t_data data;

	if (ac < 5 || ac > 6)
		return (1);
	if (init_structs(&data, ac, av))
		return (1);
	if (init_threads(&data))
		return (1);
	return 0;
}
