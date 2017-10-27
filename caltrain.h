#ifndef CALTRAIN_H_
#define CALTRAIN_H_


#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "terminal_colors.h"

struct station {
    pthread_mutex_t t_mutex;
    pthread_cond_t train_move_cond, train_arrival_cond;
    int waiting_passengers;
    int free_seats;
    int passengers_entered_train;
};

/**
 * initlize station struct.
 * 
 * @param station station struct
 */
void station_init(struct station *station);

/**
 * @brief when a train arrives in the station and has opened its doors, it invokes the function.
 * @details this function will return after all passengers are succesfully loaded or the there is no more free seats.
 * 
 * @param station station struct
 * @param count number of seats are available in the train.
 */
void station_load_train(struct station *station, int count);

/**
 * @brief When passenger arrives the station it invokes this function.
 * @details this function will return when the train is on the station and there is a free seat for the passenger.
 * 
 * @param station station struct
 */
void station_wait_for_train(struct station *station);

/**
 * Once the paasenger is seated on the train it will call this function.
 * 
 * @param station station struct
 */
void station_on_board(struct station *station);

#endif // CALTRAIN_H_
