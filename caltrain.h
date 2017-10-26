#ifndef CALTRAIN_H_
#define CALTRAIN_H_


#include <pthread.h>

#include "terminal_colors.h"

struct station {
	pthread_cond_t train_cond;
	pthread_cond_t x;
	pthread_mutex_t train_mutex;
	int passengers_on_station; // num of passenger on station
	int passengers_on_board; // num of passenger on train
	int passengers_count; // num of passenger on tain + on station
	int free_seats;
	int taken_seats;
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
