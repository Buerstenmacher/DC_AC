#include "pico/stdlib.h"
#include <time.h>


//external members of raspberry-pi-pico library:
static inline absolute_time_t get_absolute_time(void);
static inline uint64_t to_us_since_boot(absolute_time_t t);
typedef unsigned int uint;

double mashinetime(void) {	//should give us the number of seconds since boot
static double ret;
ret = (double)(to_us_since_boot(get_absolute_time()))/(double)(1000000.0);
return ret;
}

void wait_until(double time) {	//should wait until time seconds after boot
do {} while(mashinetime()<time);
}

double period_time(double freq) {return (double)(1.0)/freq;} //time for one full cycle

double duty_cycle(double startuptime) {
static double ret;
ret = mashinetime()/startuptime;
return (ret >= 1.0)?1.0:ret;
}

double on_time(double freq, double startuptime) {
double half_wave = period_time(freq)/2.0;
return half_wave*duty_cycle(startuptime);
}

double off_time(double freq, double startuptime) {
double half_wave = period_time(freq)/2.0;
return half_wave*(1.0-duty_cycle(startuptime));
}



int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
double freq =	50.0;
double startup =5.0;
const uint8_t LED_PIN = PICO_DEFAULT_LED_PIN;
const uint8_t PIN_0 = 0;
const uint8_t PIN_1 = 1;
gpio_init(LED_PIN);
gpio_init(PIN_0);
gpio_init(PIN_1);
gpio_set_dir(LED_PIN, GPIO_OUT);
gpio_set_dir(PIN_0, GPIO_OUT);
gpio_set_dir(PIN_1, GPIO_OUT);
double timepoint = mashinetime();
do 	{
	gpio_put(LED_PIN, 1);
	gpio_put(PIN_1, 1);
	timepoint += on_time(freq,startup);
	wait_until(timepoint);
        gpio_put(LED_PIN, 0);
	gpio_put(PIN_1, 0);
	timepoint += off_time(freq,startup);
	wait_until(timepoint);
	gpio_put(PIN_0, 1);
	timepoint += on_time(freq,startup);
	wait_until(timepoint);
	gpio_put(PIN_0, 0);
	timepoint += off_time(freq,startup);
	wait_until(timepoint);
	} while (1);
#endif
}
