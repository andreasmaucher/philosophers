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

/* This function takes 3 parameters in the following format:
color, action string e.g. 'TAKE_FORKS', philo;
essentially it checks if a philo died or didn't die and writes 
a message to the terminal based on that condition
*/
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
    else if (philo->data->dead == 0)
    {
        printf("%s%lu %d %s\n", color, time, philo->id, str);
    }
    pthread_mutex_unlock(&philo->data->write);
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

/* only function where the philo->data->dead value is changed to 1, 
except for the messages function where it sets it if its still 0
WHY???????????????????????????????????????????????????????????????? */
void	*meals_monitor(void *data_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) data_pointer;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (philo->data->finished >= philo->data->n_philos) //! is this just used as a terminate condition without somebody actually dying?
			philo->data->dead = 1; //! meaning I only do this if they ate enough meals?!
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

/*responsible for monitoring philos state;
Check 1: if current time has exceeded the the death_time of the philo and if 
the philo is currently not eating -> DIED;
Check 2: checks if one philo has completed the required amount of meals*/
void	*supervisor(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->death_time && philo->eating == 0) //! PRINT THIS!
			messages(RED, DIED, philo);
		if (philo->n_eat_times == philo->data->n_meals)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++; //! only initialized here
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
