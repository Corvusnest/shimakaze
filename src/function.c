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
	static char buffer[] = "              ";
	time_t temp = time(NULL); 
  	struct tm *tick_time = localtime(&temp);

	strftime(buffer, sizeof("              "), "%a     %b %e", tick_time);
	text_layer_set_text(s_date_layer, buffer);
}

void animate_shimakaze(BitmapLayer *bitmap_layer) {

	Layer *layer = bitmap_layer_get_layer(bitmap_layer);

	GRect start = GRect(86, 54, 51, 101);
	GRect finish = GRect(86, 38, 51, 101);
	PropertyAnimation *prop_anim_move_1 = property_animation_create_layer_frame(layer, &start, &finish);
	Animation *anim_move_1 = property_animation_get_animation(prop_anim_move_1);
	animation_set_duration(anim_move_1, 400);
	animation_set_curve(anim_move_1, AnimationCurveEaseOut);
	animation_set_delay(anim_move_1, 100);

	start = GRect(86, 38, 51, 101);
	finish = GRect(86, 56, 51, 101);
	PropertyAnimation *prop_anim_move_2 = property_animation_create_layer_frame(layer, &start, &finish);
	Animation *anim_move_2 = property_animation_get_animation(prop_anim_move_2);
	animation_set_duration(anim_move_2, 400);
	animation_set_curve(anim_move_2, AnimationCurveEaseIn);

	// Create two sequence animations
	Animation *seq = animation_sequence_create(anim_move_1, anim_move_2, NULL);

	// Create a spawn animation using the two sequences, set play count and begin
	//Animation *spawn = animation_spawn_create(seq, NULL);
	animation_set_play_count(seq, 4);
	animation_schedule(seq);
}

