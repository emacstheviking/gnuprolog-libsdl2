#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gprolog.h>
#include <SDL2/SDL.h>

#include "static_data.h"




// Uncomment for useful output when tinkering
#define __DEBUG__


// Debugging helpers
#define DSTR(L,X) fprintf(stdout, "%s => %s\n", L, X);
#define DHEX(L,X) fprintf(stdout, "%s => %08X\n", L, (unsigned int)X);
#define DULONG(L,X) fprintf(stdout, "%s => %lu\n", L, (unsigned long)X);



/**
 * Helper macro for SDL error return from predicate.
 */
#define RETURN_SDL_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, SDL_GetError());	\
  return PL_FALSE;


//====================================================================
// Event utilities: creating the compound term to describe the event.
//====================================================================
#define EV(F) F(&ev, &szTerm[0])
#define EVB(F) F(&ev, &szTerm[0]); break
#define EVWRAPPER(F) void F(SDL_Event *e, char* t)

EVWRAPPER(evKbd);
EVWRAPPER(evTextEditing);
EVWRAPPER(evTextInput);
EVWRAPPER(evQuit);
EVWRAPPER(evWindow);
EVWRAPPER(evDropEvent);
EVWRAPPER(evMouseButton);
EVWRAPPER(evMouseMotion);
EVWRAPPER(evMouseWheel);

const char* evWindowType(int);



/**
 * Perform standard SDL library initialisation.
 *
 * @param PlLong flags mask
 *
 * @return PL_TRUE | PL_FALSE
 *
 */
PlBool gp_SDL_Init(PlLong flags)
{
  if (flags > 0)
  {
    if (0 == SDL_Init(flags))
    {
      return PL_TRUE;
    }
    RETURN_SDL_FAIL(SDL_Init);
  }
  return PL_FALSE;
}


/**
 * Shutdown the SDL library.
 *
 * @return PL_TRUE
 *
 */
PlBool gp_SDL_Quit()
{
  SDL_Quit();

  return PL_TRUE;
}


/**
 * SDL_CreateWindow
 *
 * Create a window of a given size, position and attributes.
 *
 * @param char*  title   atom or string containing the window title
 * @param PlLong x       centered | undefined | <integer-value>
 * @param PlLong y       centered | undefined | <integer-value>
 * @param PlLong w       width of the window
 * @param PlLong h       height of the window
 * @param PlLong flags   one or more SDL_WindowFlags as a bit mask
 * @param PlTerm handle  an UNINSTANTIATED variable to hold the handle
 *
 * @return PL_TRUE | PL_FALSE
 *
 */
PlBool gp_SDL_CreateWindow(char*  title,
		       PlLong xpos,  PlLong ypos,
		       PlLong width, PlLong height,
		       PlLong flags, PlLong *handle)
{
  SDL_Window* wnd = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

  if (wnd)
  {
    *handle = (PlLong)wnd;

#ifdef __DEBUG__
    fprintf(stderr, "gp_SDL_CreateWindow: %p\n", wnd);
#endif
    return PL_TRUE;
  }

  RETURN_SDL_FAIL(SDL_CreateWindow);
}


/**
 * SDL_DestroyWindow
 *
 * Use this function to destroy a window.
 *
 * @param PlLong handle  references the window to be killed
 *
 * @return PL_TRUE
 *
 */
PlBool gp_SDL_DestroyWindow(PlLong handle)
{
  SDL_DestroyWindow((SDL_Window*)handle);

  return PL_TRUE;
}


/**
 * SDL_SetWindowTitle
 *
 * Use this function to set the title of a window.
 *
 * @param PlLong handle to a valid window
 * @param char*  title string to use
 *
 * @return PL_TRUE
 */
PlBool gp_SDL_SetWindowTitle(PlLong wnd, char* title)
{
    SDL_SetWindowTitle((SDL_Window*)wnd, title);

#ifdef __DEBUG__
    fprintf(stderr, "gp_SDL_SetWindowTitle: %p => %s\n",
	    (SDL_Window*)wnd, title);
#endif

    return PL_TRUE;
}


/**
 * SDL_CreateRenderer
 *
 * Use this function to create a 2D rendering context for a window.
 *
 * @param PlTerm handle  a valid window handle from gp_create_window
 * @param PlLong index   index of the rendering driver
 * @param PlLong flags   one or more SDL_RendererFlags as a bit mask
 *
 * @return PL_TRUE | PL_FALSE
 *
 */
PlBool gp_SDL_CreateRenderer(PlLong *handle, PlLong index, PlLong flags, PlLong *r)
{
  SDL_Window   *wnd      = (SDL_Window*)handle;
  SDL_Renderer *renderer = SDL_CreateRenderer(wnd, index, flags);

  if (renderer) {
    *r = (PlLong)renderer;
    return PL_TRUE;
  }

  RETURN_SDL_FAIL(SDL_CreateRenderer);
}


/**
 * SDL_CreateWindowAndRenderer
 *
 * Use this function to create a window and default renderer.
 *
 * @param PlTerm handle  a valid window handle from gp_create_window
 * @param PlLong index   index of the rendering driver
 * @param PlLong flags   one or more SDL_RendererFlags as a bit mask
 *
 * @return PL_TRUE | PL_FALSE
 *
 */
PlBool gp_SDL_CreateWindowAndRenderer(
    PlLong width, PlLong height, PlLong flags,
    PlLong *wnd, PlLong *renderer)
{
  SDL_Window   *w;
  SDL_Renderer *r;

  if (SDL_CreateWindowAndRenderer(width, height, flags, &w, &r))
  {
    *wnd = (PlLong)wnd;
    *renderer = (PlLong)renderer;

    return PL_TRUE;
  }

  RETURN_SDL_FAIL(SDL_CreateWindowAndRenderer);
}


/**
 * SDL_DestroyRenderer
 *
 * Use this function to destroy the rendering context for a window and
 * free associated textures.
 *
 * @param PlLong handle  references the renderer to be killed
 *
 * @return PL_TRUE
 *
 */
PlBool gp_SDL_DestroyRenderer(PlLong handle)
{
  SDL_DestroyRenderer((SDL_Renderer*)handle);

  return PL_TRUE;
}


PlBool gp_SDL_SetRenderDrawColor(PlLong rndr, PlLong r, PlLong g, PlLong b, PlLong a)
{
  SDL_SetRenderDrawColor((SDL_Renderer*)rndr, r, g, b, a);

  return PL_TRUE;
}


PlBool gp_SDL_RenderClear(PlLong rndr)
{
  SDL_RenderClear((SDL_Renderer*)rndr);

  return PL_TRUE;
}


PlBool gp_SDL_RenderPresent(PlLong rndr)
{
  SDL_RenderPresent((SDL_Renderer*)rndr);

  return PL_TRUE;
}


PlBool gp_SDL_RenderDrawPoint(PlLong rndr, PlLong x, PlLong y)
{
  SDL_RenderDrawPoint((SDL_Renderer*)rndr, (int)x, (int)y);

  return PL_TRUE;
}


PlBool gp_SDL_RenderDrawLine(
    PlLong rndr,
    PlLong x1, PlLong y1,
    PlLong x2, PlLong y2)
{
  SDL_RenderDrawLine((SDL_Renderer*)rndr, (int)x1, (int)y1, (int)x2, (int)y2);

  return PL_TRUE;
}


PlBool gp_SDL_RenderDrawRect(
    PlLong rndr,
    PlLong x, PlLong y,
    PlLong w, PlLong h)
{
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SDL_RenderDrawRect((SDL_Renderer*)rndr, &rect);

  return PL_TRUE;
}


PlBool gp_SDL_RenderFillRect(
    PlLong rndr,
    PlLong x, PlLong y,
    PlLong w, PlLong h)
{
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  SDL_RenderFillRect((SDL_Renderer*)rndr, &rect);

  return PL_TRUE;
}


PlBool gp_SDL_CreateTexture_C(
    PlLong rndr,
    PlLong format, PlLong access,
    PlLong w, PlLong h,
    PlLong *texture)
{
  SDL_Texture* pTexture = SDL_CreateTexture(
      (SDL_Renderer*)rndr,
      format,
      access,
      w, h);

  if (pTexture)
  {
    *texture = (PlLong)pTexture;
    return PL_TRUE;
  }

  RETURN_SDL_FAIL(SDL_CreateTexture);
}


PlBool gp_SDL_DestroyTexture(PlLong texture)
{
  SDL_DestroyTexture((SDL_Texture*)texture);

  return PL_TRUE;
}


PlBool gp_SDL_PollEvent(PlTerm *event)
{
  SDL_Event ev;
  PlTerm term;
  char szTerm[63+1];

  if (0 == SDL_PollEvent(&ev)) {
    *event = Pl_Mk_Integer(-1);
  }
  else {
    switch(ev.type)
    {
	case SDL_KEYDOWN:
	case SDL_KEYUP:	           EVB(evKbd);
	case SDL_TEXTEDITING:      EVB(evTextEditing);
	case SDL_TEXTINPUT:        EVB(evTextInput);
	case SDL_QUIT:             EVB(evQuit);
	case SDL_WINDOWEVENT:      EVB(evWindow);
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:    EVB(evMouseButton);
	case SDL_MOUSEMOTION:      EVB(evMouseMotion);
	case SDL_MOUSEWHEEL:       EVB(evMouseWheel);
	case SDL_DROPFILE:         EVB(evDropEvent);

	default:
	  sprintf(szTerm, "unhandled(%u)", ev.type);
    }

    term = Pl_Read_From_String(szTerm);
    Pl_Copy_Term(event, &term);

#ifdef __DEBUG__
    fprintf(stdout, "sdl_PollEvent: %s\n", szTerm);
#endif
  }
  return PL_TRUE;
}


PlBool gp_SDL_GetTicks(PlLong *ticks)
{
  *ticks = (PlLong)SDL_GetTicks();

  return PL_TRUE;
}


/**
 * SDL_Delay
 *
 * Delay the process for a number of milliseconds.
 *
 * @param  PlLong   the required delay
 *
 * @return PL_TRUE
 */
PlBool gp_SDL_Delay(PlLong delay_in_milliseconds)
{
  SDL_Delay(delay_in_milliseconds);

  return PL_TRUE;
}


PlBool gp_SDL_ShowSimpleMessageBox_C(PlLong flags, char* title, char* message)
{
  SDL_ShowSimpleMessageBox(flags, title, message, NULL);

  return PL_TRUE;
}


//====================================================================
// Event utilities: creating the compound term to describe the event.
//====================================================================

EVWRAPPER(evQuit) {
  sprintf(t,"quit(%u)", e->quit.timestamp);}


EVWRAPPER(evKbd) {
  sprintf(t,
	  "%s(%u,%u,%s,%s,%u,%u,%u)",
	  e->type == SDL_KEYDOWN ? "keydown" : "keyup",
	  e->key.timestamp,
	  e->key.windowID,
	  e->key.state == SDL_PRESSED ? "pressed" : "released",
	  e->key.repeat ? "repeat" : "first",
	  e->key.keysym.scancode,
	  e->key.keysym.sym,
	  e->key.keysym.mod); }


EVWRAPPER(evTextEditing) {
  // TODO: dynamic allocation for large strings!
  sprintf(t,
	  "text_editing(%u,%u,%s,%i,%i)",
	  e->edit.timestamp,
	  e->edit.windowID,
	  e->edit.text,
	  e->edit.start,
	  e->edit.length); }


EVWRAPPER(evTextInput) {
  // TODO: dynamic allocation for large strings!
  sprintf(t,
	  "text_input(%u,%u,%s)",
	  e->text.timestamp,
	  e->text.windowID,
	  e->text.text); }


EVWRAPPER(evWindow) {
  sprintf(t,
	  "window(%u,%u,%s)",
	  e->window.timestamp,
	  e->window.windowID,
	  evWindowType(e->window.event)); }


EVWRAPPER(evDropEvent) {
#ifdef __DEBUG__
  fprintf(stdout, "DROP FILE: %s\n", e->drop.file);
#endif
  sprintf(t,
	  "dropfile(%u,%s)",
	  e->drop.timestamp,
	  e->drop.file
	  ); // TODO: Buffer sizwe for long filenames!
}


EVWRAPPER(evMouseButton) {
  sprintf(t,
	  "%s(%u,%u,%u,%i,%s,%i,%i,%i)",
	  e->type == SDL_MOUSEBUTTONDOWN ? "mouse_down" : "mouse_up",
	  e->button.timestamp,
	  e->button.windowID,
	  e->button.which,
	  e->button.button,
	  e->button.state == SDL_PRESSED ? "pressed" : "released",
	  e->button.clicks,
	  e->button.x,
	  e->button.y); }


EVWRAPPER(evMouseMotion) {
  sprintf(t,
	  "mouse_motion(%u,%u,%u,%u,%i,%i,%i,%i)",
	  e->motion.timestamp,
	  e->motion.windowID,
	  e->motion.which,
	  e->motion.state,
	  e->motion.x,
	  e->motion.y,
	  e->motion.xrel,
	  e->motion.yrel
	  ); }


EVWRAPPER(evMouseWheel) {
  sprintf(t,
	  "mouse_wheel(%u,%u,%u,%i,%i)",
	  e->motion.timestamp,
	  e->motion.windowID,
	  e->motion.which,
	  e->motion.x,
	  e->motion.y); }


//--------------------------------------------------------------------
// Wrapper helper functions...
//--------------------------------------------------------------------
const char* evWindowType(int id) {
  switch(id) {
      case SDL_WINDOWEVENT_SHOWN: return "shown";
      case SDL_WINDOWEVENT_HIDDEN: return "hidden";
      case SDL_WINDOWEVENT_EXPOSED: return "exposed";
      case SDL_WINDOWEVENT_MOVED: return "moved";
      case SDL_WINDOWEVENT_RESIZED: return "resized";
      case SDL_WINDOWEVENT_SIZE_CHANGED: return "size_changed";
      case SDL_WINDOWEVENT_MINIMIZED: return "minimized";
      case SDL_WINDOWEVENT_MAXIMIZED: return "maximized";
      case SDL_WINDOWEVENT_RESTORED: return "restored";
      case SDL_WINDOWEVENT_ENTER: return "enter";
      case SDL_WINDOWEVENT_LEAVE: return "leave";
      case SDL_WINDOWEVENT_FOCUS_GAINED: return "focus_gained";
      case SDL_WINDOWEVENT_FOCUS_LOST: return "focus_lost";
      case SDL_WINDOWEVENT_CLOSE: return "close";
      default: return "unknown";
  }
}
