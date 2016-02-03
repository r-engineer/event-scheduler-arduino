#include "scheduler.h"
#include <Arduino.h>
#include "blinky_led.h"
#include "FlexiTimer2.h"

blinky_led led1(2, 500, 1); 
blinky_led led2(3, 750, 1);
blinky_led led3(4, 1250, 1);

void schedGpioFunc(uint8_t lvl);

void setup() 
{
	pinMode(7, OUTPUT);
	
	led1.init();
	led1.enable();
	led2.init();
	led2.enable();
	led3.init();
	led3.enable();
	
	scheduler_o.add_event(&led1);
	scheduler_o.add_event(&led2);
	scheduler_o.add_event(&led3);
	scheduler_o.init(schedGpioFunc);
	scheduler_o.timer_start();
}

void loop() 
{ 
	scheduler_o.execute_events();
}

void schedGpioFunc(uint8_t lvl)
{
	digitalWrite(7, lvl);
}