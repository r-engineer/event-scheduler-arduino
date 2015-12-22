#include "scheduler.h"
#include "FlexiTimer2.h"
#include "Arduino.h"

scheduler scheduler_o;

polled_event::polled_event()
{
	count_start = 0;
	current_count = 0;
	enable_flag = false;
	next = 0;
}

polled_event::polled_event(uint16_t cnt_start)
{
	//
	count_start = cnt_start;
	current_count = cnt_start;
	enable_flag = false;
	next = 0;
}

void polled_event::reset_counter()
{
	//don't let the timer interrupt modify the counter while updating 
	//the counter but also need to restore the proper enable value
	noInterrupts();
	current_count = count_start;
	interrupts();
	
}

//automatically resets current_count but doesn't modify enable
void polled_event::set_counter(uint16_t cnt_start)
{
	//don't let the timer interrupt modify the counter while updating 
	//the counter but also need to restore the proper enable value
	noInterrupts();
	count_start = cnt_start;
	interrupts();
}

void polled_event::enable()
{
	enable_flag = true;
}
void polled_event::disable()
{
	enable_flag = false;
}

bool polled_event::get_enable()
{
	return enable_flag;
}
			
void polled_event::decrement()
{	
	if(enable_flag)
	{
		if(current_count > 0)
		{
			current_count--;
		}
	}
}		

uint16_t polled_event::get_current_count()
{
	return current_count;
}
		

/**************/
/**************/
scheduler::scheduler()
{
	rootptr = 0;
}


//in function the address of event is the same as the address
//of the passed objec (or it should be at least)
//
void scheduler::add_event(polled_event *event) 
{
	polled_event *tempptr = rootptr;
	
	noInterrupts();
	
	if(tempptr == 0)
	{
		rootptr = event;
	}
	else
	{
		while(tempptr->next != 0)
		{
			tempptr = tempptr->next;
		}
		tempptr->next = event;
		//make sure this new event pointst to nothing
		event->next = 0;
	}
	
	interrupts();

}

void scheduler::init()
{
	
	FlexiTimer2::set(1, 1.0/1000, update_counts); // call every 500 1ms "ticks"
	// FlexiTimer2::set(500, flash); // MsTimer2 style is also supported
	
}


void scheduler::execute_events()
{
	uint16_t current_count;
	polled_event *tempptr;
	
	tempptr = rootptr;
	while(tempptr != 0)
	{
		if(tempptr->get_enable())
		{
			noInterrupts();
			current_count = tempptr->get_current_count();
			interrupts();
			if(current_count == 0)
			{
				digitalWrite(13, HIGH);
				tempptr->execute();
				digitalWrite(13, LOW);
			}
		}
		tempptr = tempptr->next;
	}
}

inline void update_counts()
{
	polled_event *tempptr;
	
	tempptr = scheduler_o.rootptr;
	while(tempptr != 0)
	{
		tempptr->decrement();
		tempptr = tempptr->next;
	}
}

void scheduler::timer_start()
{
	FlexiTimer2::start();
}
