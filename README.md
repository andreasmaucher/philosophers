# philosophers - a 42 project

<p align="center">
<img src="https://github.com/andreasmaucher/philosophers/assets/57959338/05404cf7-427f-49ad-a01d-a4b9d3f1923f" alt="Image description" width="420"/>
</p>

## Description
This project is about multithreading with the use of mutexes and semaphores. There is a table of n philosophers (1 philospher = 1 thread) that share a common routine (eating, sleeping, thinking). Each of them has one fork, but in order to eat they need to hold two forks. Communication between philosophers is not allowed. The main challenges are handling shared resources among the threads and avoiding data races. Philosophers are represented by threads and forks by mutexes.

## Installation
Compile executable with executing make.

## Usage
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> <[number_of_times_each_philosopher_must_eat]>

Example: ./philo 5 500 100 100
