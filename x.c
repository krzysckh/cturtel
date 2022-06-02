#include "turtel.h"

#ifdef GRAPHICS

#include <X11/Xlib.h>

static Display *display;
static Window win;
static GC gc;
static Colormap cmap;
static int fastcolor;

#endif

void x_create_win(int X, int Y) {
#ifdef GRAPHICS
  display = XOpenDisplay(0);
  if (!display) {
    err("runtime: cannot open display\n");
  }

  Visual *visual = DefaultVisual(display, 0);
  if (visual && visual->class == TrueColor) {
    fastcolor = 1;
  } else {
    fastcolor = 0;
  }

  int black = BlackPixel(display, DefaultScreen(display));
  int white = WhitePixel(display, DefaultScreen(display));

  win = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0,
      X, Y, 0, black, black);

  XSetWindowAttributes attr;
  attr.backing_store = Always;
  XChangeWindowAttributes(display, win, CWBackingStore, &attr);

  XStoreName(display, win, "trl window");

  XSelectInput(display, win, StructureNotifyMask|KeyPressMask|ButtonPressMask);

  XMapWindow(display, win);

  gc = XCreateGC(display, win, 0, 0);

  cmap = DefaultColormap(display, 0);

  XSetForeground(display, gc, white);

  for (;;) {
    XEvent e;
    XNextEvent(display, &e);

    if (e.type == MapNotify)
      break;
  }
#else
  err("runtime: compiled without graphics\n");
#endif
}

void x_draw(int x, int y, int r, int g, int b) {
#ifdef GRAPHICS
  /* just yoinked from gfx */
  XColor color;

  if (fastcolor) {
    color.pixel = ((b&0xff) | ((g&0xff)<<8) | ((r&0xff)<<16) );
  } else {
    color.pixel = 0;
    color.red = r<<8;
    color.green = g<<8;
    color.blue = b<<8;
    XAllocColor(display, cmap, &color);
  }

  XSetForeground(display, gc, color.pixel);

  XDrawPoint(display, win, gc, x, y);

  XFlush(display);
#else
  err("runtime: compiled without graphics");
#endif
}

void x_del_win() {
#ifdef GRAPHICS
  XCloseDisplay(display);
#else
  err("runtime: compiled without graphics\n");
#endif
}
