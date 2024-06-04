/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <amaucher@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/23 10:16:54 by amaucher          #+#    #+#             */
/*   Updated: 2023/08/23 10:16:56 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>

enum ErrorCodes 
{
    MEM_ALLOC_ERROR,
	THREAD_ERR,
	THREAD_JOIN_ERR,
	INPUT_ERR,
	CURRENT_TIME_ERR,
	SUCCESS,
};

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

//! why size_t could I do int?!
//! Explain the how the two structs are connected!!!!!!
/*
It's common in the dining philosophers problem to use two sets of mutexes for forks:
one set associated with the philosophers and another set in the form of an array,
often called forks, to represent the actual forks on the table. This separation allows
for independent management of the forks and the philosophers' interactions with them.
*/
typedef struct s_philo
{
    pthread_t		thread;
	int				id;
	bool			eating;
	int				meals_counter;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				n_philos;
	int				num_times_to_eat;
	int				finished;
	bool			*dead;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
}					t_philo;

typedef struct s_data
{
	t_philo			*philos;
	bool			dead_flag;
	int				n_philos;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t **forks;
}					t_data;

/* input check */
int			valid_input(char **av);

/* initialization */
void		init_structs(t_philo *philos, t_data *data, char **av);

/* utils */
int			ft_atoi(const char *nptr);
size_t		ft_strlen(const char *s);
u_int64_t	current_time(void);
int			ft_usleep(u_int64_t time_ms);
int			free_and_destroy(int err_code, t_data *data);

/* threads */
int			create_threads(t_data *data);
void		*routine(void *philo_pointer);

/* supervisor */
bool		check_dead_flag(t_philo *philo);
bool		check_if_all_ate(t_philo *philos);
bool		philosopher_dead(t_philo *philo, size_t time_to_die);
void		*supervisor(void *pointer);

/* actions */
void		eating(t_philo *philo);
void		sleeping(t_philo *philo);
void		thinking(t_philo *philo);
void		message(char *color, char *str, t_philo *philo);


#endif
