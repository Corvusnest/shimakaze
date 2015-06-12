#include <pebble.h>
#include "function.h"

static Window *s_main_window;

static GBitmap *s_shimakaze_bitmap;
static BitmapLayer *s_shimakaze_layer;
static GBitmap *s_floor_bitmap;
static BitmapLayer *s_floor_layer;
static GBitmap *s_window_bitmap;
static BitmapLayer *s_window_layer;
static Layer *s_line_layer;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;

/***************** TRY *****************/
static GBitmap *s_redraw_bitmap;
static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, s_redraw_bitmap, GRect(0, 0, gbitmap_get_bounds(s_redraw_bitmap).size.w, gbitmap_get_bounds(s_redraw_bitmap).size.h));
}

static void line_update_proc(Layer *this_layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect(0, 0, 73, 1), 0, GCornerNone);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(s_time_layer);
}

/***************** START *****************/
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  //Floor
  s_floor_bitmap = gbitmap_create_with_resource(RESOURCE_ID_FLOOR);
  s_floor_layer = bitmap_layer_create(GRect(0, 154, 144, 14));
  bitmap_layer_set_bitmap(s_floor_layer, s_floor_bitmap);
  bitmap_layer_set_compositing_mode(s_floor_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_floor_layer));

  //Shimakaze
  s_shimakaze_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SHIMAKAZE);
  s_shimakaze_layer = bitmap_layer_create(GRect(86, 56, 51, 101));
  bitmap_layer_set_bitmap(s_shimakaze_layer, s_shimakaze_bitmap);
  bitmap_layer_set_compositing_mode(s_shimakaze_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_shimakaze_layer));

  //Window
  s_window_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WINDOW_DAY);
  s_window_layer = bitmap_layer_create(GRect(21, 22, 47, 54));
  bitmap_layer_set_bitmap(s_window_layer, s_window_bitmap);
  bitmap_layer_set_compositing_mode(s_window_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_window_layer));

  //Time line
  s_line_layer = layer_create(GRect(6, 113, 73, 1));
  layer_add_child(window_layer, s_line_layer);
  layer_set_update_proc(s_line_layer, line_update_proc);

  //layer_set_hidden(bitmap_layer_get_layer(s_shimakaze_layer), true);
  //s_redraw_bitmap = s_shimakaze_bitmap;
  //layer_set_update_proc(bitmap_layer_get_layer(s_shimakaze_layer), canvas_update_proc);

  layer_insert_above_sibling(bitmap_layer_get_layer(s_shimakaze_layer), bitmap_layer_get_layer(s_floor_layer));

  //Time
  s_time_layer = text_layer_create(GRect(1, 114, 84, 28));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_28_LIGHT_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  // Make sure the time is displayed from the start
  update_time(s_time_layer);

  //Date
  s_date_layer = text_layer_create(GRect(1, 100, 84, 14));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_text(s_date_layer, "               ");
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_09));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  update_date(s_date_layer);
}

static void main_window_unload(Window *window) {
  bitmap_layer_destroy(s_shimakaze_layer);
  bitmap_layer_destroy(s_floor_layer);
  bitmap_layer_destroy(s_window_layer);
  text_layer_destroy(s_time_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorCobaltBlue);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}