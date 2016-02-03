#ifndef SCHEDULER_H    
#define SCHEDULER_H  

#define USE_UTILIZE_FUNC 1

#include <stdint.h>

#define MAX_NUMBER_EVENTS 10

class polled_event
{
public:

	polled_event(uint16_t cnt_start);
	polled_event();
	void reset_counter();
	void enable();
	void disable();
	virtual void execute() = 0;
	virtual void init() = 0;
	void decrement();
	bool get_enable();
	uint16_t get_current_count();
	polled_event *next;
	
protected:
	volatile bool enable_flag;
	volatile uint16_t current_count;
	uint16_t count_start;
	void set_counter(uint16_t cnt_start);
private:
	
};

void update_counts();

class scheduler
{
public:
	scheduler();
	void add_event(polled_event *event); //in function the address of event is the same as the address
											//of the passed objec (or it should be at least)
											//
	void execute_events();
	void timer_start();//start
	void init(void(*func)(uint8_t));
	void init();
	friend void update_counts();
private:
	polled_event *rootptr;
	void (*gpioFunc)(uint8_t);
};

/******************************************************************************
Need to create an object so that the update_counts() function has something
to work with.  I would like to pass an object reference to update_counts() function 
to avoid making it a friend function and to avoid creating the scheduler_o object
in this file but the callback function passed to FlexiTimer2 is defined as a 
void (*func)() which has to match the definition of update_counts()
******************************************************************************/
extern scheduler scheduler_o;

#endif