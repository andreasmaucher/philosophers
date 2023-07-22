/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amaucher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 10:57:24 by amaucher          #+#    #+#             */
/*   Updated: 2023/07/21 10:57:26 by amaucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "data.h"

pthread_mutex_t mutex;
int mails = 0;

void* routine() {
    for (int i = 0; i < 10000000; i++) {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char* argv[]) {
    pthread_t th[8];
    int i;
	int	*res;
    pthread_mutex_init(&mutex, NULL);
	i = 0;
    while (i < 8) 
	{
        if (pthread_create(th + i, NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        printf("Thread %d has started\n", i);
		i++;
    }
	i = 0;
    while (i < 8)
	{
        if (pthread_join(th[i], (void**) &res) != 0) {
            return 2;
        }
        printf("Thread %d has finished execution\n", i);
		i++;
    }
    pthread_mutex_destroy(&mutex);
    printf("Number of mails: %d\n", mails);
    return 0;
}
