#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

//print macros
#define DIGITS CENTI_POWER//hardcoded to centiseconds, this macro will not change anything
#define DIGIT_ROUNDING CENTI_IN_SECONDS

//number of nanoseconds to wait before update
#define TIME_RESOLUTION (NANO_IN_SECONDS/CENTI_IN_SECONDS)


//part-second macros
#define NANO_IN_SECONDS 1000000000
#define NANO_POWER 9

#define MICRO_IN_SECONDS 1000000
#define MICRO_POWER 6

#define MILLI_IN_SECONDS 1000
#define MILLI_POWER 3

#define CENTI_IN_SECONDS 100
#define CENTI_POWER 2


//state macros
#define COUNTING 0
#define PAUSED 1
#define UNPAUSED 2
#define RESET 3
#define START_RESET 4
#define WAIT_RESET 5
int state = RESET;


struct timespec time_sub(struct timespec t1, struct timespec t2)
{
	struct timespec diff;
	time_t sec_nsec_carry = 0;
	if (t2.tv_nsec > t1.tv_nsec){sec_nsec_carry = 1;}
	diff.tv_sec = t1.tv_sec - t2.tv_sec - sec_nsec_carry;
	diff.tv_nsec = (t1.tv_nsec - t2.tv_nsec + NANO_IN_SECONDS) % NANO_IN_SECONDS;
	return diff;
}

struct timespec get_time_since(struct timespec start)
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC_RAW, &now);
	return time_sub(now, start);
}

void print_time(struct timespec timer)
{
	long int nanos = timer.tv_nsec;
	long int centis = timer.tv_nsec/(NANO_IN_SECONDS/CENTI_IN_SECONDS);
	long int seconds = timer.tv_sec % 60;
	long int minutes = (timer.tv_sec / 60) % 60;
	long int hours = timer.tv_sec / 3600;
	if (hours > 0){
		printf("\f\n\n%ld:%.02ld:%.02ld.%.02ld\n", hours, minutes, seconds, centis);
	} else if (minutes > 0){
		printf("\f\n\n%ld:%.02ld.%.02ld\n", minutes, seconds, centis);
	} else {
		printf("\f\n\n%.1ld.%.02ld\n", seconds, centis);
	}
}






void start_stopwatch() { if (state==PAUSED) {state = UNPAUSED;} else if (state==WAIT_RESET){state=START_RESET;}}
void pause_stopwatch() { if (state==COUNTING) {state = PAUSED;}}
void reset_stopwatch() { state = RESET;}


void signal_handler(int the_signal)
{
	if (the_signal == SIGUSR1){
		start_stopwatch();
		return;
	} else if (the_signal == SIGUSR2){
		pause_stopwatch();
		return;
	} else if (the_signal == SIGHUP){
		reset_stopwatch();
	}

}

int main(int argc, char *argv[])
{
	struct timespec start;
	struct timespec time_elapsed;
	struct timespec now;
	time_elapsed.tv_sec = (time_t) 0;
	time_elapsed.tv_nsec = (long int) 0;
	struct timespec sleep_time;
	sleep_time.tv_sec = (time_t) 0;
	sleep_time.tv_nsec = (long int) TIME_RESOLUTION;
	
	//register signals
	struct sigaction signal_handler_struct;
	memset(&signal_handler_struct, 0, sizeof(signal_handler_struct));
	signal_handler_struct.sa_handler = signal_handler;
	signal_handler_struct.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR1, &signal_handler_struct, NULL)){
		fprintf(stderr, "Couldn't register SIGUSR1 unpause handler.\n");
	}
	if (sigaction(SIGUSR2, &signal_handler_struct, NULL)){
		fprintf(stderr, "Couldn't register SIGUSR2 pause handler.\n");
	}
	if (sigaction(SIGHUP, &signal_handler_struct, NULL)){
		fprintf(stderr, "Couldn't register SIGHUP reset handler.\n");
	}
	
	
	
	//start clock
	clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	
	while(1){
		switch (state){
			case PAUSED:
				break;
			case UNPAUSED:
				clock_gettime(CLOCK_MONOTONIC_RAW, &now);
				start = time_sub(now, time_elapsed);
				state = COUNTING;
				break;
			case RESET:
				printf("\f\n\n0.00\n");
				fflush(stdout);
				state = WAIT_RESET;
				break;
			case WAIT_RESET:
				break;
			case START_RESET:
				clock_gettime(CLOCK_MONOTONIC_RAW, &start);
				state = COUNTING;
				break;
			case COUNTING:
				time_elapsed = get_time_since(start);
				print_time(time_elapsed);
				//note, if you don't flush the buffer it will wait to store a full page before flushing
				fflush(stdout);
				break;
			}
			nanosleep(&sleep_time, NULL);
	}
}
