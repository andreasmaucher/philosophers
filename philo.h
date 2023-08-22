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
# include <sys/time.h>
# include <stdint.h>
# include <unistd.h>

// Define ANSI codes for colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32;1m" 
#define YELLOW  "\x1b[33;1m"
#define BLUE    "\x1b[34;1m" 
#define GREY 	"\x1b[90m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//	philo_msg
/*ANSI_COLOR_RESET ensures, that the next message is uncolored if not specified otherwise*/
# define TAKE_FORKS "has taken a fork" ANSI_COLOR_RESET
# define THINKING   "is thinking" ANSI_COLOR_RESET
# define SLEEPING   "is sleeping" ANSI_COLOR_RESET
# define EATING     "is eating" ANSI_COLOR_RESET
# define DIED       "died" ANSI_COLOR_RESET

//	alloc_err
# define ALLOC_ERR_1 "ERROR WHILE ALLOCATING THREADS IDs"
# define ALLOC_ERR_3 "ERROR WHILE ALLOCATING PHILOS"
# define ALLOC_ERR_2 "ERROR WHILE ALLOCATING FORKS"
//	input_err
# define INPUT_ERROR "INVALID INPUT"
//	pthread_err
# define TH_ERR "ERROR WHILE CREATING THREADS"
# define JOIN_ERR "ERROR WHILE JOINING THREADS"
# define INIT_ERR_1 "ERROR WHILE INIT FORKS"
//	time_err
# define TIME_ERR "UNABLE TO RETRIVE UTC"

enum ErrorCodes {
    MEM_ALLOC_ERROR,
	THREAD_ERR,
	THREAD_JOIN_ERR,
};

/* typedef enum e_data_state
{
	EATING = 0,
	SLEEPING = 1,
	THINKING = 2,
	DEAD = 3,
}	t_state; */

struct	s_data;

//typedef long			t_ms;

typedef struct s_philo
{
	struct s_data	*data;
	int				id;
	int			philo_id;
	int			n_eat_times;
	u_int64_t		death_time;
	pthread_t		supervisor;
	int				status;
	int				eating;
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
	u_int64_t	time_to_die;
	u_int64_t	time_to_eat;
	u_int64_t	time_to_sleep;
	u_int64_t	start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;

}	t_data;

int	error(char *str, t_data *data);

/*time*/
u_int64_t	get_time(void);

/*actions*/
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

/*utils*/
int	ft_atoi(const char *nptr);
int	ft_usleep(u_int64_t time);
int	ft_strcmp(char *s1, char *s2);
int	valid_input(char **argv);
int	error(char *str, t_data *data);
void	destroy_mutexes(t_data *data);
void	free_memory(t_data	*data);

/*threads*/
int	handle_threads(t_data *data);

/*initialization*/
int init_structs(t_data *data, int ac, char **av);

/*main*/
void	*routine(void *philo_pointer);
void	*meals_monitor(void *data_pointer);
void messages(char *color, char *str, t_philo *philo);
void *death_monitor(void *data_pointer);

#endif
