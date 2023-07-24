/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 12:47:35 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/24 12:47:44 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
