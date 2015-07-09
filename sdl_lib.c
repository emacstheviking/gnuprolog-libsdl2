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
	case SDL_KEYUP:
	  sprintf(szTerm,
	  	  "%s(%u,%u,%s,%s,%u,%u,%u)",
	  	  ev.type == SDL_KEYDOWN ? "keydown" : "keyup",
	  	  ev.key.timestamp,
	  	  ev.key.windowID,
	  	  ev.key.state == SDL_PRESSED ? "pressed" : "released",
	  	  ev.key.repeat ? "repeat" : "first",
	  	  ev.key.keysym.scancode,
	  	  ev.key.keysym.sym,
	  	  ev.key.keysym.mod);
	  break;

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






PlBool arse(PlTerm x)
{
  //  Uint32 = term_atom_or_positive(x, gstr_CwPos, gbit_CwPos);
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
