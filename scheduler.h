#ifndef SCHEDULER_H    
#define SCHEDULER_H  

#include "stdint.h"

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
	void set_counter(uint16_t cnt_start); //automatically resets current_count but doesn't modify enable
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
	void init();
	friend void update_counts();
private:
	polled_event *rootptr;
};

extern scheduler scheduler_o;

#endif