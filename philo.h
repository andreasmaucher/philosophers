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

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h> // time
# include <stdint.h> // uint64_t
# include <unistd.h> // usleep

// Define ANSI escape codes for colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32;1m" 
#define YELLOW  "\x1b[33;1m"
#define BLUE    "\x1b[34;1m" 
#define GREY 	"\x1b[90m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//	alloc_err
# define ALLOC_ERR_1 "ERROR WHILE ALLOCATING THREADS IDs"
# define ALLOC_ERR_3 "ERROR WHILE ALLOCATING PHILOS"
# define ALLOC_ERR_2 "ERROR WHILE ALLOCATING FORKS"
//	input_err
# define ERR_IN_1 "INVALID INPUT CHARACTER"
# define ERR_IN_2 "INVALID INPUT VALUES"
//	pthread_err
# define TH_ERR "ERROR WHILE CREATING THREADS"
# define JOIN_ERR "ERROR WHILE JOINING THREADS"
# define INIT_ERR_1 "ERROR WHILE INIT FORKS"
//	time_err
# define TIME_ERR "UNABLE TO RETRIVE UTC"
//	philo_msg
/*ANSI_COLOR_RESET ensures, that the next message is uncolored if not specified otherwise*/
# define TAKE_FORKS "has taken a fork" ANSI_COLOR_RESET
# define THINKING   "is thinking" ANSI_COLOR_RESET
# define SLEEPING   "is sleeping" ANSI_COLOR_RESET
# define EATING     "is eating" ANSI_COLOR_RESET
# define DIED       "died" ANSI_COLOR_RESET

/* typedef enum e_data_state
{
	EATING = 0,
	SLEEPING = 1,
	THINKING = 2,
	DEAD = 3,
}	t_state; */

struct	s_data;

typedef long			t_ms;

typedef struct s_philo
{
	struct s_data	*data;
	int				id;
	int			philo_id;
	int			n_eat_times;
	t_ms		death_time;
	pthread_t		t1;
	int				status;
	int				eating;
	//t_state		state;
	pthread_mutex_t	lock;
	pthread_mutex_t		*fork_l;
	pthread_mutex_t		*fork_r;

}	t_philo;

typedef struct s_data
{
	t_philo		*philo;
	pthread_t	*id;
	pthread_t	*philo_threads;
	int			n_philos;
	int			n_meals;
	int			dead;
	int			finished;
	t_ms	time_to_die;
	t_ms	time_to_eat;
	t_ms	time_to_sleep;
	t_ms	start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;

}	t_data;

int	error(char *str, t_data *data);

/*time*/
t_ms	get_time(void);

/*utils*/
int	ft_atoi(const char *nptr);
int	ft_usleep(t_ms time);
int	ft_strcmp(char *s1, char *s2);
int	input_checker(char **argv);


/*initialization*/
int init_structs(t_data *data, int ac, char **av);

#endif
