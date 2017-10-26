#include "caltrain.h"

int suppress_logs = 0;

void
station_init(struct station *station)
{
	pthread_cond_init(&(station->train_cond), NULL);
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
	// init station values
	station->passengers_on_board = 0;
	station->taken_seats = 0;
	station->free_seats = count;
	station->passengers_count = station->passengers_on_station + station->passengers_on_board;
	pthread_cond_broadcast(&(station->train_cond));
	if (!suppress_logs) {
		printf(ANSI_COLOR_BLUE "******* broadcast sent to %d passengers *******\n" ANSI_COLOR_RESET "\n", station->passengers_on_station);
		printf(ANSI_COLOR_RED "######## train arrived with free_seats = %d, passengers_on_station = %d" ANSI_COLOR_RESET "\n", station->free_seats, station->passengers_on_station);
	}	
	while (station->passengers_on_station != 0 && station->free_seats != 0) {
		pthread_cond_broadcast(&(station->train_cond));
		if (!suppress_logs)
		printf(ANSI_COLOR_BLUE "******* broadcast sent to %d passengers *******\n" ANSI_COLOR_RESET "\n", station->passengers_on_station);
	}

	if (!suppress_logs)
	printf(ANSI_COLOR_CYAN "######## train should move with free_seats = %d, passengers_on_station = %d, passengers on board = %d" ANSI_COLOR_RESET "\n", station->free_seats, station->passengers_on_station, station->passengers_on_board);
	while (station->passengers_on_board != station->taken_seats) {
		if (!suppress_logs)
		printf("waiting passenger to have seats, station_on_board = %d, taken_seats = %d\n", station->passengers_on_board, station->taken_seats);
	}
	if (!suppress_logs)
	printf(ANSI_COLOR_GREEN "######## TRAIN MOVED" ANSI_COLOR_RESET "\n");
}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&(station->train_mutex)); // if it's already locked it will wait until it becomes free.
	// CRITICAL SECTION
	station->passengers_count++;
	station->passengers_on_station++;
	if (!suppress_logs)
	printf("######## new passenger! passengers_count = %d passengers_on_station = %d\n", station->passengers_count, station->passengers_on_station);
	pthread_cond_wait(&(station->train_cond), &(station->train_mutex));
	if (!suppress_logs)
	printf("******* broadcast received! ******* free_seats = %d\n", station->free_seats);
	while (station->free_seats == 0) { // no free seats 
		if (!suppress_logs)
		printf("passenger is waiting for free seat\n");
		pthread_cond_wait(&(station->train_cond), &(station->train_mutex)); // wait until another train come.
	}

	station->passengers_on_station--; // move towards the train
	station->free_seats--; // reserve a seat
	station->taken_seats++; // reserve a seat
	if (!suppress_logs)
	printf(ANSI_COLOR_MAGENTA "######## passenger will enter the train! passengers_count = %d, passengers_on_station = %d, passengers_on_board = %d" ANSI_COLOR_RESET "\n", station->passengers_count, station->passengers_on_station, station->passengers_on_board);
	pthread_mutex_unlock(&(station->train_mutex));
}

void
station_on_board(struct station *station)
{
	station->passengers_on_board++;
	if (!suppress_logs)
	printf(ANSI_COLOR_YELLOW "######## passenger on board! passengers_count = %d, passengers_on_station = %d, passengers_on_board = %d, taken_seats = %d" ANSI_COLOR_RESET "\n", station->passengers_count, station->passengers_on_station, station->passengers_on_board, station->taken_seats);
}
