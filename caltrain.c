#include "caltrain.h"

void
station_init(struct station *station)
{
    station->waiting_passengers = 0;
    station->free_seats = 0;
    station->passengers_entered_train = 0;
    pthread_mutex_init(&(station->t_mutex), NULL);
    pthread_cond_init(&(station->train_move_cond), NULL);
    pthread_cond_init(&(station->train_arrival_cond), NULL);
}
 
void
station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&(station->t_mutex));
    station->free_seats = count;
    pthread_cond_broadcast(&(station->train_arrival_cond));
    // we check waiting_passengers count so we don't wait for no one
    // also we must check that all passenger are successully on board
    // checking only the first condition is wrong because there may be still waiting_passengers and  
    // free seats on the train after broadcast and we must make sure
    // all passengers entered the train are ready and on board so the second condition is there.
    //
    // also checking only second condition is wrong because there may be no waiting passengers or no
    // free seats and the train will wait forever.
    //
    while ((station->waiting_passengers != 0 && station->free_seats != 0) || station->passengers_entered_train != 0)
        pthread_cond_wait(&(station->train_move_cond), &(station->t_mutex));
    station->free_seats = 0;
    pthread_mutex_unlock(&(station->t_mutex));
}
 
void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&(station->t_mutex));
    station->waiting_passengers++;
    while(station->free_seats == 0)
        pthread_cond_wait(&(station->train_arrival_cond), &(station->t_mutex));
    station->free_seats--;
    station->waiting_passengers--;
    station->passengers_entered_train++;
    pthread_mutex_unlock(&(station->t_mutex));
}
 
void
station_on_board(struct station *station)
{  
    pthread_mutex_lock(&(station->t_mutex));
    station->passengers_entered_train--;
    if(station->passengers_entered_train == 0) // finish processing all passengers
        pthread_cond_signal(&(station->train_move_cond));
    pthread_mutex_unlock(&(station->t_mutex));
}
