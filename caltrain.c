#include "caltrain.h"


void
station_init(struct station *station)
{
	pthread_cond_init(&(station->train_cond), NULL);
	pthread_cond_init(&(station->x), NULL);
	pthread_mutex_init(&(station->train_mutex), NULL);
	pthread_mutex_init(&(station->m1), NULL);
	pthread_mutex_init(&(station->m2), NULL);

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

	printf(ANSI_COLOR_RED "######## train arrived with free_seats = %d, passengers_on_station = %d" ANSI_COLOR_RESET "\n", station->free_seats, station->passengers_on_station);
	if (station->passengers_on_station != 0 && station->free_seats != 0) {
		pthread_cond_broadcast(&(station->train_cond));
		pthread_cond_wait(&(station->x), &(station->m1));
	}
	printf(ANSI_COLOR_CYAN "######## train should move with free_seats = %d, passengers_on_station = %d, passengers on board = %d" ANSI_COLOR_RESET "\n", station->free_seats, station->passengers_on_station, station->passengers_on_board);
	while (station->passengers_on_board != station->passengers_count - station->passengers_on_station);
	printf(ANSI_COLOR_GREEN "######## TRAIN MOVED" ANSI_COLOR_RESET "\n");
}

void
station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&(station->train_mutex)); // if it's already locked it will wait until it becomes free.
	// CRITICAL SECTION
	station->passengers_count++;
	station->passengers_on_station++;
	printf("######## new passenger! passengers_count = %d passengers_on_station = %d\n", station->passengers_count, station->passengers_on_station);
	pthread_cond_wait(&(station->train_cond), &(station->train_mutex));
	printf("******* broadcast sent! ******* free_seats = %d\n", station->free_seats);
	while (station->free_seats == 0) { // no free seats 
		pthread_cond_signal(&(station->x));
		pthread_cond_wait(&(station->train_cond), &(station->train_mutex)); // wait until another train come.
	}

	station->passengers_on_station--; // move towards the train
	if (station->passengers_on_station == 0) {
		pthread_cond_signal(&(station->x));
	}
	station->free_seats--; // reserve a seat
	station->taken_seats++; // reserve a seat
	printf(ANSI_COLOR_MAGENTA "######## passenger will enter the train! passengers_count = %d, passengers_on_station = %d, passengers_on_board = %d" ANSI_COLOR_RESET "\n", station->passengers_count, station->passengers_on_station, station->passengers_on_board);
	pthread_mutex_unlock(&(station->train_mutex));
}

void
station_on_board(struct station *station)
{
	station->passengers_on_board++;
	printf(ANSI_COLOR_YELLOW "######## passenger on board! passengers_count = %d, passengers_on_station = %d, passengers_on_board = %d" ANSI_COLOR_RESET "\n", station->passengers_count, station->passengers_on_station, station->passengers_on_board);
}
