/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 10:58:39 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/21 10:58:41 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef data_H
# define data_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h> // time
# include <stdint.h> // uint64_t
# include <unistd.h> // usleep

typedef enum e_data_state
{
	EATING = 0,
	SLEEPING = 1,
	THINKING = 2,
	DEAD = 3,
}	t_state;

typedef struct s_philo
{
	int			philo_id;
	int			n_eat_times;
	uint64_t	time_to_die;
	t_state		state;
	pthread_mutex_t		*fork_l;
	pthread_mutex_t		*fork_r;

}	t_philo;

typedef struct s_data
{
	int			thread_id;
	pthread_t	*philo_threads;
	int			n_philos;
	uint64_t	time_to_die;
	uint64_t	time_to_eat;
	uint64_t	time_to_sleep;
	int			n_eat_times;
	uint64_t	start_time;
	pthread_mutex_t	*forks;	

}	t_data;

#endif
