/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 12:11:22 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/14 12:11:24 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "philo.h"

/* own implementation of the atoi function */
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

/* my own implementation of the strcmp function */
int ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	
	while(s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{	
		i++;
	}
	return (s1[i] - s2[i]);
}

//! FIX
int	error(char *str, t_data *data)
{
	data = 0;
	printf("%s\n", str);
	//if (data)
	//	destroy_mutexes(data);
	return (1);
}

/* tv stands for timeval and returns current time; returns time 
in milliseconds*/
t_ms	get_time(void)
{
	struct timeval	tv;
	t_ms 			time_in_ms;

	if (gettimeofday(&tv, NULL))
		return (0);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

/* don't fully understands why this works yet */
int	ft_usleep(t_ms time)
{
	t_ms	start;

	start = get_time();
	while ((get_time() - start) < time)
		usleep(1000);
	return (0);
}


/* initializing the philo struct; id is incremented by +1 so that 
the id of the first philo is 1; memory allocation already happened in the 
init data struct function below*/
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
		pthread_mutex_init(&data->philo[i].lock, NULL); //! MUTEXES PHILO->LOCK in a loop!
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

	i = 0;
	while (i < data->n_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL); //! MUTEXES FOR EACH FORK
		i++;
	}
	i = 0;
	data->philo[0].fork_l = &data->forks[0];
	data->philo[0].fork_r = &data->forks[data->n_philos - 1];
	i++;
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
	data->start_time = 0;
	data->dead = 0;
	data->finished = 0;
	data->id = malloc(sizeof(pthread_t) * data->n_philos);
	if (data->id == NULL)
		return (MEM_ALLOC_ERROR);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->n_philos);
	if (data->forks == NULL)
		return (MEM_ALLOC_ERROR);
	data->philo = malloc(sizeof(t_philo) * data->n_philos);
	if (data->philo == NULL)
		return (MEM_ALLOC_ERROR);
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

/* 
1. setting the start time
2. creating threads for each philosopher while executing the routine function
(if n_meals parameter is set a meals_monitor thread is created o check when all philos have
eaten n_meals); the first argument is to store the id of the thread & the last argument
is a pointer to the data that gets passed
3. after the routine is finished the threads are joined;
*/
int	handle_threads(t_data *data)
{
	int			i;
	pthread_t	meals_monitor_thread;

	data->start_time = get_time();
	if (data->n_meals > 0)
	{
		if (pthread_create(&meals_monitor_thread, NULL, &meals_monitor, &data->philo[0]))
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


/* This function takes 3 parameters in the following format:
color, action string e.g. 'TAKE_FORKS', philo;
essentially it checks if a philo died or didn't die and writes 
a message to the terminal based on that condition;
1 Check: if message = DIED -> set philo to dead;
2 Check: if philo is not dead -> print philos action/status;
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
	printf("N EAT TIMES %d\n", philo->n_eat_times);
	//ft_usleep(philo->data->time_to_eat); //! Purpose?
	philo->eating = 0;
	pthread_mutex_unlock(&philo->lock);
	drop_forks(philo);
}

/* only function where the philo->data->dead value is changed to 1, 
except for the messages function where it sets it if its still 0
WHY???????????????????????????????????????????????????????????????? */
void *meals_monitor(void *data_pointer)
{
    t_philo *philo;

    philo = (t_philo *)data_pointer;
    while (1)
    {
        pthread_mutex_lock(&philo->lock);
        int total_meals = 0;
        for (int i = 0; i < philo->data->n_philos; i++)
        {
            total_meals += philo->data->philo[i].n_eat_times;
        }
        if (total_meals >= philo->data->n_philos * philo->data->n_meals)
        {
            philo->data->dead = 1;
            pthread_mutex_unlock(&philo->lock);
            break; // Terminate the loop
        }
        pthread_mutex_unlock(&philo->lock);
    }
    return (NULL);
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
			printf("%d\n", philo->data->finished);
			philo->n_eat_times++;
			printf("%d\n", philo->n_eat_times);
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
