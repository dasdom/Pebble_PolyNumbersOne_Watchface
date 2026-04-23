#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_large_hour_layer;
static TextLayer *s_large_minutes_layer;
static TextLayer *s_date_layer;
static GFont s_time_font;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_time_buffer[248];
  strftime(s_time_buffer, sizeof(s_time_buffer), clock_is_24h_style() ?
           "%H%M%H%M%H%M%H%M%H%M%H%M%H%M%H%M"
           "%H%M%H%M%H%M%H%M%H%M%H%M%H%M%H%M"
           "%H%M%H%M%H%M%H%M%H%M%H%M%H%M%H%M"
           "%H%M%H%M%H%M%H%M%H%M%H%M%H%M%H%M"
           "%H%M%H%M%H%M%H%M%H%M%H%M%H%M%H%M"
           "%H%M%H%M%H%M%H%M%H%M%H%M%H%M%H%M"
           "%H%M%H%M%H%M%H%M%H%M%H%M%H%M%H%M"
           "%H%M%H%M%H%M%H%M%H%M%H%M%H%M%H%M"
           : 
           "%I%M%I%M%I%M%I%M%I%M%I%M%I%M%I%M"
           "%I%M%I%M%I%M%I%M%I%M%I%M%I%M%I%M"
           "%I%M%I%M%I%M%I%M%I%M%I%M%I%M%I%M"
           "%I%M%I%M%I%M%I%M%I%M%I%M%I%M%I%M"
           "%I%M%I%M%I%M%I%M%I%M%I%M%I%M%I%M"
           "%I%M%I%M%I%M%I%M%I%M%I%M%I%M%I%M"
           "%I%M%I%M%I%M%I%M%I%M%I%M%I%M%I%M"
           "%I%M%I%M%I%M%I%M%I%M%I%M%I%M%I%M"
           , tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_time_buffer);
    
  static char s_large_hour_buffer[4];
  strftime(s_large_hour_buffer, sizeof(s_large_hour_buffer), clock_is_24h_style() ? "%H" : "%I", tick_time);
    
  text_layer_set_text(s_large_hour_layer, s_large_hour_buffer);

  static char s_large_minutes_buffer[4];
  strftime(s_large_minutes_buffer, sizeof(s_large_minutes_buffer), "%M", tick_time);
    
  text_layer_set_text(s_large_minutes_layer, s_large_minutes_buffer);

  // Write the current date into a buffer
  static char s_date_buffer[16];
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %d", tick_time);

  // Display the date
  text_layer_set_text(s_date_layer, s_date_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
    
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CHANGA_ONE_75));

  // Create the time TextLayer
  s_time_layer = text_layer_create(GRect(-10, -10, bounds.size.w + 30, PBL_IF_ROUND_ELSE(bounds.size.h-30, bounds.size.h-20)));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_LECO_20_BOLD_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  int  y_hour_offset, y_minutes_offset;
  
  printf("%d", watch_info_get_model());
  printf("%d", PBL_DISPLAY_HEIGHT);
  
  if (PBL_DISPLAY_HEIGHT < 170) {
    y_hour_offset = -10;
    y_minutes_offset = 50;
  } else if (PBL_DISPLAY_HEIGHT < 190) {
    y_hour_offset = -12;
    y_minutes_offset = 44;
  } else if (PBL_DISPLAY_HEIGHT < 250) {
    y_hour_offset = 10;
    y_minutes_offset = 70;
  } else {
    y_hour_offset = 25;
    y_minutes_offset = 90;
  }
  
  // Create the hour TextLayer
  s_large_hour_layer = text_layer_create(GRect(0, y_hour_offset, bounds.size.w, 90));
  text_layer_set_background_color(s_large_hour_layer, GColorClear);
  text_layer_set_text_color(s_large_hour_layer, GColorBlack);
  text_layer_set_font(s_large_hour_layer, s_time_font);
  text_layer_set_text_alignment(s_large_hour_layer, GTextAlignmentCenter);

  // Create the minutes TextLayer
  s_large_minutes_layer = text_layer_create(GRect(0, y_minutes_offset, bounds.size.w, 90));
  text_layer_set_background_color(s_large_minutes_layer, GColorClear);
  text_layer_set_text_color(s_large_minutes_layer, GColorBlack);
  text_layer_set_font(s_large_minutes_layer, s_time_font);
  text_layer_set_text_alignment(s_large_minutes_layer, GTextAlignmentCenter);
    
  // Create the date TextLayer
  s_date_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(bounds.size.h-46, bounds.size.h-36), bounds.size.w, 30));
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);

  // Add layers to the Window
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_large_hour_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_large_minutes_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_large_hour_layer);
  text_layer_destroy(s_large_minutes_layer);
  text_layer_destroy(s_date_layer);
  fonts_unload_custom_font(s_time_font);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set the background color
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
