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

int	error(char *str, t_data *data)
{
	data = 0;
	printf("%s\n", str);
	//if (data)
		//ft_exit(data);
	return (1);
}

bool	is_in_strings(char c, char *str)
{
	while (*str)
	{
		if (*str == c)
			return (true);
		str++;
	}
	return (false);
}

int	input_checker(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == ' ')
			{
				j++;
				continue ;
			}
			if ((argv[i][j] < 48 || argv[i][j] > 57))
				return (error(ERR_IN_1, NULL));
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_usleep(t_ms time)
{
	t_ms	start;

	start = get_time();
	while ((get_time() - start) < time)
		usleep(time / 10);
	return (0);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 != '\0' && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(char *)s1 - *(char *)s2);
}

void	messages(char *str, t_philo *philo)
{
	t_ms	time;

	pthread_mutex_lock(&philo->data->write);
	time = get_time() - philo->data->start_time;
	if (ft_strcmp(DIED, str) == 0 && philo->data->dead == 0)
	{
		printf("%lu %d %s\n", time, philo->id, str);
		philo->data->dead = 1;
	}
	if (!philo->data->dead)
		printf("%lu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write);
}

void	take_forks(t_philo *philo)
{
	/* pthread_mutex_lock(philo->fork_r);
	messages(TAKE_FORKS, philo);
	pthread_mutex_lock(philo->fork_l);
	messages(TAKE_FORKS, philo); */
	if (philo->id % 2 == 1) // Odd philosopher
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
    }
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->fork_l);
	pthread_mutex_unlock(philo->fork_r); //!
	messages(SLEEPING, philo);
	ft_usleep(philo->data->time_to_sleep);
}

void	eat(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->lock);
	philo->eating = 1;
	philo->death_time = get_time() + philo->data->time_to_die;
	messages(EATING, philo);
	philo->n_eat_times++;
	ft_usleep(philo->data->time_to_eat);
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	drop_forks(philo);
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

/*responsible for monitoring philos state*/
void	*supervisor(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	while (philo->data->dead == 0)
	{
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->death_time && philo->eating == 0)
			messages(DIED, philo);
		if (philo->n_eat_times == philo->data->n_meals)
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++;
			philo->n_eat_times++;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

/* supervisor routine that checks if the philo had died or finished eating;
dead = 0 is the main loop, as long as all philos are alive philos will eat;
Once the loop is exited the function waits 
for the supervisor thread to finish its job using pthread_join(philo->t1, 
NULL). This ensures that the main thread waits for the supervisor thread to 
complete before proceeding*/
void	*routine(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer; //! WHY
	philo->death_time = philo->data->time_to_die + get_time();
	if (pthread_create(&philo->t1, NULL, &supervisor, (void *)philo))
		return ((void *)1);
	while (philo->data->dead == 0)
	{
		eat(philo);
		messages(THINKING, philo);
	}
	if (pthread_join(philo->t1, NULL)) //!
		return ((void *)1);
	return ((void *)0);
}

/* initializing & creating threads; if n_meals parameter is set monitor
thread is created o check when all philos have eaten n_meals*/
int	handle_threads(t_data *data)
{
	int			i;
	pthread_t	t0;

	i = -1;
	data->start_time = get_time(); //! DO I NEED TO START COUNTING HERE OR AFTER CREATING THE THREADS
	if (data->n_meals > 0)
	{
		if (pthread_create(&t0, NULL, &monitor, &data->philo[0]))
			return (error(TH_ERR, data));
	}
	while (++i < data->n_philos)
	{
		if (pthread_create(&data->id[i], NULL, &routine, &data->philo[i]))
			return (error(TH_ERR, data));
		ft_usleep(100); //! necessary?
	}
	i = -1;
	while (++i < data->n_philos)
	{
		if (pthread_join(data->id[i], NULL))
			return (error(JOIN_ERR, data));
	}
	return (0);
}

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
