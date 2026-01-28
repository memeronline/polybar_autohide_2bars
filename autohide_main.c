#include "autohide.h"
#include "autohide_aux.c"
#include <stdio.h>
#include <xdo.h>

Window *getBar(xdo_t *xdo, unsigned int *found_bars) {
  Window *bar = NULL;
  Window **list = malloc(sizeof(Window *) * 20);
  xdo_search_t query;
  unsigned int n_win = 0;

  // initialize query to find bar window
  memset(&query, 0, sizeof(xdo_search_t));
  query.max_depth = -1;
  query.require = SEARCH_ANY;
  query.searchmask |= SEARCH_CLASS;
  query.winclass = BAR_WM_CLASS;

  xdo_search_windows(xdo, &query, list, found_bars);
  bar = list[0];

  printf("%u", *found_bars);
  free(list);
  return bar;
}

int activeWindowIsFullscreen(xdo_t *xdo) {
  int fullscreen = 0;
  Window window;
  long nitems;
  unsigned char *states;

  int ret = xdo_get_active_window(xdo, &window);
  if (ret == XDO_ERROR || window == 0)
    fail("ERROR: xdo_get_active_window");

  ret = xdo_get_window_property(xdo, window, "_NET_WM_STATE", &states, &nitems,
                                NULL, NULL);
  if (ret == XDO_ERROR)
    fail("ERROR: xdo_get_window_property");

  char *state;
  for (int i = 0; i < nitems; ++i) {
    state = XGetAtomName(xdo->xdpy, ((Atom *)states)[i]);
    if (strcmp(state, "_NET_WM_STATE_FULLSCREEN") == 0)
      fullscreen = 1;
    XFree(state);
  }
  free(states);

  return fullscreen;
}

int main() {
  // init xdo
  xdo_t *xdo = xdo_new(NULL);
  unsigned int found_bars = 0;
  if (xdo == NULL) {
    fail("ERROR: Failed to initialize xdo");
  }
  int y = 0;

  // get bar window object
  Window *bar = getBar(xdo, &found_bars);
  Window top_bar = 0, bottom_bar = 0;

  // failsafe
  if (bar == NULL) {
    fail("ERROR: Failed to find bar");
  }
  // save gotten PID's as either the top or the bottom bar
  for (int i = 0; i < found_bars; ++i) {
    xdo_get_window_location(xdo, bar[i], NULL, &y, NULL);
    if (y <= MARGIN_TOP / 2) {
      top_bar = bar[i];
    } else {
      bottom_bar = bar[i];
    }
  }

  // main loop
  bool hidden = false;
  while (1) {
    // check if active window is fullscreen
    if (hidden && CHECK_FULLSCREEN && activeWindowIsFullscreen(xdo)) {
      usleep(LOOP_DELAY * 1000);
      continue;
    }

    // get mouse location
    xdo_get_mouse_location(xdo, NULL, &y, NULL);

    // if mouse is not in top or bottom bar
    if (y > MARGIN_HIDE_TOP && y < MARGIN_HIDE_BOTTOM) {
      if (!hidden) {
        xdo_unmap_window(xdo, top_bar);
        xdo_unmap_window(xdo, bottom_bar);
        hidden = true;
      }
      usleep(CURSOR_WINDOW_DELAY * 1000);
      continue;
    }

    // modified by memer.online, basically if someone by chance stumbles upon my
    // own config, I have both a top AND a bottom polybar, I removed the
    // individual checks and just combined them into an OR statement
    // if mouse is in top or bottom bar
    else if (y < MARGIN_TOP || y > MARGIN_BOTTOM) {
      xdo_map_window(xdo, top_bar);
      xdo_map_window(xdo, bottom_bar);
      usleep(BAR_HIDE_DELAY * 1000);
      hidden = false;
      continue;
    }

    usleep(LOOP_DELAY);
  }

  xdo_free(xdo);
  return 0;
}

// memer.online rant:
// p.s. I fucking **HATE** XDOTools documentation, what should've been a simple
// fucking 15 minute hack job turned into an hour+ long manhunt to try and even
// FIGURE OUT what some of these fucking functions do
// (NOT TO MENTION ALL THE BRAINSTORMING MY UTTER DUMBASS HAD TO DO FOR THE
// LOGIC)
// ((AS WELL AS THE FACT I HAD TO FIGURE OUT EXACTLY WHAT THAT STUPID WINDOW
// CLASS STORED EXACTLY oh my fucking god Im never touching C again))
