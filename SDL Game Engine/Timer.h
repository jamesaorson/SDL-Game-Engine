#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>
#include "Definitions.h"

//The application time based timer
class Timer {
public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	int getTicks();

	//Checks the status of the timer
	bool isStarted();
	bool isPaused();

private:
	//The clock time sincee the timer started
	int startTicks;

	//The ticks stored while timer was paused
	int pausedTicks;

	bool paused;
	bool started;
};

#endif //TIMER_H