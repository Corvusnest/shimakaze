#include <pebble.h>
#include "function.h"

void update_time(TextLayer *s_time_layer) {
	static char buffer[] = "00:00";
	time_t temp = time(NULL); 
	struct tm *tick_time = localtime(&temp);
 
	if(clock_is_24h_style() == true) {
		strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
	} 
	else {
	    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
	}
	text_layer_set_text(s_time_layer, buffer);
}

void update_date(TextLayer *s_date_layer) {
	static char buffer[] = "               ";
	time_t temp = time(NULL); 
  	struct tm *tick_time = localtime(&temp);

	strftime(buffer, sizeof("               "), "%a     %b  %e", tick_time);
	text_layer_set_text(s_date_layer, buffer);
}