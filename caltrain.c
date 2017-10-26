#include "caltrain.h"


void
station_init(struct station *station)
{
	pthread_cond_init(&(station->train_cond), NULL);
	pthread_cond_init(&(station->x), NULL);
	pthread_mutex_init(&(station->train_mutex), NULL);
	station->passengers_count = 0;
	station->passengers_on_station = 0;
	station->passengers_on_board = 0;
	station->free_seats = 0;
	station->taken_seats = 0;
}

void
station_load_train(struct station *station, int count)
{
	// init values
	station->passengers_on_board = 0;
	station->taken_seats = 0;
	station->free_seats = count;
	station->passengers_count = station->passengers_on_station;

	pthread_cond_broadcast(&(station->train_cond));
	printf(ANSI_COLOR_RED "######## train arrived with free_seats = %d" ANSI_COLOR_RESET "\n", station->free_seats);
	// pthread_mutex_lock(&(station->train_mutex));
	while(station->free_seats != 0 && station->passengers_on_station != 0) { 
		// waits until there is no free seats OR all passengers on station has gone
		// pthread_cond_wait(&(station->train_cond), &(station->train_mutex));
	}
	printf(ANSI_COLOR_CYAN "######## train should move with free_seats = %d, passengers_on_station = %d, passengers on board = %d" ANSI_COLOR_RESET "\n", station->free_seats, station->passengers_on_station, station->passengers_count - station->passengers_on_station);
	while (station->passengers_on_board != station->passengers_count - station->passengers_on_station);
	printf(ANSI_COLOR_GREEN "######## TRAIN MOVED" ANSI_COLOR_RESET "\n");
}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&(station->train_mutex));
	// CRITICAL SECTION
	station->passengers_count++;
	station->passengers_on_station++;
	printf("######## new passenger! passengers_count = %d\n", station->passengers_count);
	pthread_cond_wait(&(station->train_cond), &(station->train_mutex));
	while(station->free_seats == 0) { // no free seats 
		// pthread_mutex_unlock(&(station->train_mutex));
		// pthread_cond_wait(&(station->train_cond), &(station->train_mutex));
	}
	// pthread_cond_signal(&(station->train_cond));
	// pthread_mutex_lock(&(station->train_mutex));
	station->passengers_on_station--;
	station->free_seats--;
	station->taken_seats++;
	printf(ANSI_COLOR_MAGENTA "######## passenger will enter the train! passengers_count = %d, passengers_on_station = %d, passengers_on_board = %d" ANSI_COLOR_RESET "\n", station->passengers_count, station->passengers_on_station, station->passengers_on_board);
	// pthread_mutex_unlock(&(station->train_mutex));
	pthread_cond_signal(&(station->train_cond));
}

void
station_on_board(struct station *station)
{
	// pthread_mutex_lock(&(station->train_mutex));
	station->passengers_on_board++;
	printf(ANSI_COLOR_YELLOW "######## passenger on board! passengers_count = %d, passengers_on_station = %d, passengers_on_board = %d" ANSI_COLOR_RESET "\n", station->passengers_count, station->passengers_on_station, station->passengers_on_board);
	// pthread_mutex_unlock(&(station->train_mutex));
}