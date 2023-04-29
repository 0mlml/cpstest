#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

const int WINDOW_WIDTH = 320;
const int WINDOW_HEIGHT = 320;
const char *WINDOW_TITLE = "CPS Test";

Display *display;
Window window;

bool create_window()
{
  display = XOpenDisplay(NULL);
  if (display == NULL)
  {
    printf("Cannot open display\n");
    return false;
  }

  int screen = DefaultScreen(display);
  window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, 1,
                               BlackPixel(display, screen), WhitePixel(display, screen));
  XStoreName(display, window, WINDOW_TITLE);

  XSelectInput(display, window, ExposureMask | ButtonPressMask);

  XMapWindow(display, window);
  return true;
}

void close_window()
{
  XCloseDisplay(display);
}

int main()
{
  if (!create_window())
  {
    printf("Failed to create window!\n");
    return 1;
  }

  XEvent event;
  int click_count = 0;
  unsigned long start_time = time(NULL);

  while (true)
  {
    XNextEvent(display, &event);

    if (event.type == ButtonPress)
    {
      if (event.xbutton.button == Button1)
      {
        click_count++;
      }
    }
    else if (event.type == ClientMessage)
    {
      break;
    }

    unsigned long elapsed_time = time(NULL) - start_time;
    if (elapsed_time >= 1)
    {
      if (click_count > 0)
      {
        printf("CPS: %d\n", click_count);
      }
      click_count = 0;
      start_time = time(NULL);
    }
  }

  close_window();
  return 0;
}
