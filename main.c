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

//! FIX
int	error(char *str, t_data *data)
{
	data = 0;
	printf("%s\n", str);
	//if (data)
	//	ft_exit(data);
	return (1);
}

// Updated messages function with color parameter
void messages(char *color, char *str, t_philo *philo)
{
    t_ms time;

    pthread_mutex_lock(&philo->data->write);
    time = get_time() - philo->data->start_time;
    if (ft_strcmp(DIED, str) == 0 && philo->data->dead == 0)
    {
        printf("%s%lu %d %s\n", color, time, philo->id, str);
        philo->data->dead = 1;
    }
    else if (!philo->data->dead)
    {
        printf("%s%lu %d %s\n", color, time, philo->id, str);
    }
    pthread_mutex_unlock(&philo->data->write);
}

//! try with adjustments
void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_r);
	messages(YELLOW, TAKE_FORKS, philo);
	pthread_mutex_lock(philo->fork_l);
	messages(YELLOW, TAKE_FORKS, philo);
	/* if (philo->id % 2 == 1) // Odd philosopher
    {
        pthread_mutex_lock(philo->fork_l);
        messages(TAKE_FORKS, philo);
        pthread_mutex_lock(philo->fork_r);
    }
    else // Even philosopher
    {
        pthread_mutex_lock(philo->fork_r);
        messages(TAKE_FORKS, philo);
        pthread_mutex_lock(philo->fork_l);
    } */
}

/* unlocking both forks and setting the philo to sleep */
void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->fork_l);
	pthread_mutex_unlock(philo->fork_r);
	messages(GREY, SLEEPING, philo);
	ft_usleep(philo->data->time_to_sleep);
}

/* only mutex the usage, but holding at the same time is possible? */
void	eat(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->death_time = get_time() + philo->data->time_to_die;
	messages(GREEN, EATING, philo);
	philo->n_eat_times++;
	//ft_usleep(philo->data->time_to_eat); //! Purpose?
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	drop_forks(philo);
}

void	*monitor(void *data_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) data_pointer;
	//pthread_mutex_lock(&philo->data->write);
	//printf("data val: %d", philo->data->dead);
	//pthread_mutex_unlock(&philo->data->write);
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->data->finished >= philo->data->n_philos)
			philo->data->dead = 1;
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

/*responsible for monitoring philos state;
Check 1: if current time has exceeded the the death_time of the philo and if 
the philo is currently not eating -> is so DIED;
Check 2: checks if one philo has completed the required amount of meals*/
void	*supervisor(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->death_time && philo->eating == 0)
			messages(RED, DIED, philo);
		if (philo->n_eat_times == philo->data->n_meals)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++;
			philo->n_eat_times++;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
	}
	return (NULL);
}

/* supervisor routine that checks if the philo had died or finished eating;
dead = 0 is the main loop, as long as all philos are alive philos will eat;
Once the loop is exited the function waits 
for the supervisor thread to finish its job using pthread_join(philo->supervisor, 
NULL). This ensures that the main thread waits for the supervisor thread to 
complete before proceeding.
@param philo_pointer expects a pointer to a philo struct to identify each 
distinct philosopher */
void	*routine(void *philo_pointer)
{
	t_philo	*philo;
	
	philo = (t_philo *) philo_pointer;
	philo->death_time = philo->data->time_to_die + get_time();
	if (pthread_create(&philo->supervisor, NULL, &supervisor, &philo))
		return (NULL);
	while (philo->data->dead == 0)
	{
		eat(philo);
		messages(BLUE, THINKING, philo);
	}
	if (pthread_join(philo->supervisor, NULL))
		return (NULL);
	return (0);
}

/* 
1. setting the start time
2. creating threads for each philosopher while executing the routine function
(if n_meals parameter is set a monitor thread is created o check when all philos have
eaten n_meals); the first argument is to store the id of the thread & the last argument
is a pointer to the data that gets passed
3. after the routine is finished the threads are joined;
*/
int	handle_threads(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	data->start_time = get_time();
	if (data->n_meals > 0)
	{
		if (pthread_create(&monitor_thread, NULL, &monitor, &data->philo[0]))
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

//! input checks!
int	main(int ac, char *av[])
{
	t_data data;

	if (ac < 5 || ac > 6)
		return (1);
	if (init_structs(&data, ac, av))
		return (1);
	if (handle_threads(&data))
		return (1);
	return 0;
}
