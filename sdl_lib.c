#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gprolog.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "static_data.h"




// Uncomment for useful output when tinkering
#define __DEBUG__


// Debugging helpers
#define DSTR(L,X) fprintf(stdout, "%s => %s\n", L, X);
#define DHEX(L,X) fprintf(stdout, "%s => %08X\n", L, (unsigned int)X);
#define DULONG(L,X) fprintf(stdout, "%s => %lu\n", L, (unsigned long)X);


// These are created during SDL_Init to avoid repetition
int g_atomTextInput   = 0;
int g_atomTextEditing = 0;
int g_atomDropFile    = 0;
int g_atomDisplayMode = 0;


#define RETURN_SDL_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, SDL_GetError());	\
  return PL_FALSE;

#define SHOW_SDL_FAIL(F) fprintf(stderr, "%s failed -> %s\n", #F, SDL_GetError());

#define RETURN_TTF_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, TTF_GetError());	\
  return PL_FALSE;

#define SHOW_TTF_FAIL(F) fprintf(stderr, "%s failed -> %s\n", #F, TTF_GetError());

#define RETURN_MIX_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, Mix_GetError());	\
  return PL_FALSE;

#define SHOW_MIX_FAIL(F) fprintf(stderr, "%s failed -> %s\n", #F, Mix_GetError());


#define EV(F) F(&ev, &szTerm[0])
#define EVB(F) F(&ev, &szTerm[0]); break
#define EVWRAPPER(F) void F(SDL_Event *e, char* t)


// These create response with Pl_Read_From_String
EVWRAPPER(evKbd);
EVWRAPPER(evQuit);
EVWRAPPER(evWindow);
EVWRAPPER(evMouseButton);
EVWRAPPER(evMouseMotion);
EVWRAPPER(evMouseWheel);


#define EVB_TERM(F) F(&ev, &szTerm[0], &term); break
#define EVWRAPPER_TERM(F) void F(SDL_Event *e, char* t, PlTerm *o)

// These create response with Pl_Mk_Compound
EVWRAPPER_TERM(evTextEditing);
EVWRAPPER_TERM(evTextInput);
EVWRAPPER_TERM(evDropEvent);

// Maps event codes into strings for atom conversion
const char* evWindowType(int);

// render a font texture into a renderer
void renderFont(SDL_Renderer* r, SDL_Surface* pText, int x, int y);




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
  g_atomTextInput = Pl_Create_Atom("text_input");
  g_atomTextEditing = Pl_Create_Atom("text_editing");
  g_atomDropFile = Pl_Create_Atom("dropfile");
  g_atomDisplayMode = Pl_Create_Atom("display_mode");

  if (flags > 0) {
    if (0 == SDL_Init(flags)) {
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
PlBool gp_SDL_Quit() {
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

  if (wnd) {
    *handle = (PlLong)wnd;
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
 * @param PlLong handle  a valid window handle from gp_create_window
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

  if (SDL_CreateWindowAndRenderer(width, height, flags, &w, &r)) {
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

  if (pTexture) {
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
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:    EVB(evMouseButton);
	case SDL_MOUSEMOTION:      EVB(evMouseMotion);
	case SDL_MOUSEWHEEL:       EVB(evMouseWheel);
	case SDL_WINDOWEVENT:      EVB(evWindow);

	case SDL_KEYDOWN:
	case SDL_KEYUP:	           EVB(evKbd);

	case SDL_TEXTEDITING:      EVB_TERM(evTextEditing);
	case SDL_TEXTINPUT:        EVB_TERM(evTextInput);

	case SDL_QUIT:             EVB(evQuit);
	case SDL_DROPFILE:         EVB_TERM(evDropEvent);

	default:
	  sprintf(szTerm, "unhandled(%u)", ev.type);
    }

    // Only copy the term if the term string was written to
    // otherwise we assume the *handler* wrote into "term"
    if (szTerm[0]) {
      term = Pl_Read_From_String(szTerm);
    }

#ifdef __DEBUG__
    Pl_Write(term); fprintf(stdout, "\n");
#endif

    Pl_Copy_Term(event, &term);
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


/**
 * SDL_ShowSimpleMessageBox
 *
 * @param PlLong flags   simple message box flags
 * @param char*  title   C-string title of the message
 * @param char*  message C-string content of the message
 *
 * @return PL_TRUE
 */
PlBool gp_SDL_ShowSimpleMessageBox_C(PlLong flags, char* title, char* message)
{
  SDL_ShowSimpleMessageBox(flags, title, message, NULL);
  return PL_TRUE;
}


PlBool gp_SDL_StartTextInput()
{
  SDL_StartTextInput();
  return PL_TRUE;
}


PlBool gp_SDL_StopTextInput()
{
  SDL_StopTextInput();
  return PL_TRUE;
}


PlBool gp_SDL_SetTextInputRect(PlLong x, PlLong y, PlLong w, PlLong h)
{
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SDL_SetTextInputRect(&rect);
  return PL_TRUE;
}


//====================================================================
//
//                Event functors for sdl_PollEvent
//
// Each wrapper returns a functor+arguments to sdl_PollEvent(), the
// order of arguments is the same as the standard SDL documentation so
// it remains is to figure it out. Each comment header below contains
// a description of the form, where <foo> is a variable value
// extracted from the event record and things not in angle brackets
// are atoms.
//
//====================================================================


//--------------------------------------------------------------------
// quit( <timestamp> ).
//--------------------------------------------------------------------
EVWRAPPER(evQuit) { sprintf(t,"quit(%u)", e->quit.timestamp); }


//--------------------------------------------------------------------
// keydown / keyup( <timestamp>,
//                  <windowID>,
//                  pressed / released,
//                  repeat  / first,
//                  <scancode>,
//                  <sym>
//                  <mod> ).
//--------------------------------------------------------------------
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


//--------------------------------------------------------------------
// text_editing( <timestamp>, <windowID>, <text>, <start>, <length> ).
//--------------------------------------------------------------------
EVWRAPPER_TERM(evTextEditing)
{
  PlTerm args[] = {
    Pl_Mk_Integer((PlLong)e->edit.timestamp),
    Pl_Mk_Integer((PlLong)e->edit.windowID),
    Pl_Mk_Codes(e->edit.text),
    Pl_Mk_Integer(e->edit.start),
    Pl_Mk_Integer(e->edit.length)
  };
  int argc = sizeof(args) / sizeof(PlTerm);

  *t = 0;
  *o = Pl_Mk_Compound(g_atomTextEditing, 5, &args[0]);
}


//--------------------------------------------------------------------
// text_input( <timestamp>, <windowID>, <text> ).
//--------------------------------------------------------------------
EVWRAPPER_TERM(evTextInput)
{
  PlTerm timestamp  = Pl_Mk_Integer((PlLong)e->text.timestamp);
  PlTerm windowID   = Pl_Mk_Integer((PlLong)e->text.windowID);
  PlTerm textBuffer = Pl_Mk_Codes(e->text.text);
  PlTerm args[]     = {timestamp, windowID, textBuffer};

  *t = 0;
  *o = Pl_Mk_Compound(g_atomTextInput, 3, &args[0]);
}


//--------------------------------------------------------------------
// window( <timestamp>, <windowID>, <window-event> ).
//--------------------------------------------------------------------
EVWRAPPER(evWindow)
{
  sprintf(t,
	  "window(%u,%u,%s)",
	  e->window.timestamp,
	  e->window.windowID,
	  evWindowType(e->window.event));
}


//--------------------------------------------------------------------
// dropfile( <timestamp>, <filename> ).
//--------------------------------------------------------------------
EVWRAPPER_TERM(evDropEvent)
{
#ifdef __DEBUG__
  fprintf(stdout, "DROP FILE: %s\n", e->drop.file);
#endif

  PlTerm timestamp = Pl_Mk_Integer((PlLong)e->drop.timestamp);
  PlTerm filename  = Pl_Mk_Codes(e->drop.file);
  PlTerm args[]     = {timestamp, filename};

  *t = 0;
  *o = Pl_Mk_Compound(g_atomDropFile, 2, &args[0]);
}


//--------------------------------------------------------------------
// mouse_down / mouse_up ( <timestamp>,
//                         <windowID>,
//                         <which>,
//                         <button>,
//                         pressed / released,
//                         <clicks>,
//                         <x>,
//                         <y> ).
//--------------------------------------------------------------------
EVWRAPPER(evMouseButton)
{
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
	  e->button.y);
}


//--------------------------------------------------------------------
// mouse_motion ( <timestamp>,
//                <windowID>,
//                <which>,
//                <state>,
//                <x>,
//                <y>,
//                <xrel>,
//                <yrel> ).
//--------------------------------------------------------------------
EVWRAPPER(evMouseMotion)
{
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
	  );
}


//--------------------------------------------------------------------
// mouse_wheel ( <timestamp>,
//               <windowID>,
//               <which>,
//               <state>,
//               <x>,
//               <y> ).
//--------------------------------------------------------------------
EVWRAPPER(evMouseWheel) {
  sprintf(t,
	  "mouse_wheel(%u,%u,%u,%i,%i)",
	  e->motion.timestamp,
	  e->motion.windowID,
	  e->motion.which,
	  e->motion.x,
	  e->motion.y);
}


//--------------------------------------------------------------------
// Window Event -> Descriptive String for functor use.
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


//--------------------------------------------------------------------
//
//
//
//                    Circle functions
//
//
//
//--------------------------------------------------------------------

/**
 * Pseudo-SDL: SDL_RenderDrawCircle
 *
 * @param PlLong renderer  a valid handle to an SDL_Renderer*
 * @param PlLong x0        circle x-position of centre
 * @param PlLong y0        circle y-position of centre
 * @param PlLong radius    pixel radius of the circle
 *
 * @return PL_TRUE
 */
PlBool gp_SDL_RenderDrawCircle(PlLong renderer, PlLong x0, PlLong y0, PlLong radius)
{
  SDL_Renderer *rndr = (SDL_Renderer*)renderer;

  int x = radius;
  int y = 0;
  int decisionOver2 = 1 - x;

  while(x >= y)
  {
    SDL_RenderDrawPoint(rndr,  x + x0,  y + y0);
    SDL_RenderDrawPoint(rndr,  y + x0,  x + y0);
    SDL_RenderDrawPoint(rndr, -x + x0,  y + y0);
    SDL_RenderDrawPoint(rndr, -y + x0,  x + y0);
    SDL_RenderDrawPoint(rndr, -x + x0, -y + y0);
    SDL_RenderDrawPoint(rndr, -y + x0, -x + y0);
    SDL_RenderDrawPoint(rndr,  x + x0, -y + y0);
    SDL_RenderDrawPoint(rndr,  y + x0, -x + y0);

    y++;

    if (decisionOver2>0) {
      decisionOver2 += ((y - (--x))<<1)+1;
    }
    else {
      decisionOver2 += (y<<1)+1;
    }
  }
  return PL_TRUE;
}


/**
 * Pseudo-SDL: SDL_RenderFillCircle
 *
 * @param PlLong renderer  a valid handle to an SDL_Renderer*
 * @param PlLong x0        circle x-position of centre
 * @param PlLong y0        circle y-position of centre
 * @param PlLong radius    pixel radius of the circle
 *
 * @return PL_TRUE
 */
PlBool gp_SDL_RenderFillCircle(PlLong renderer, PlLong x0, PlLong y0, PlLong radius)
{
  SDL_Renderer *rndr = (SDL_Renderer*)renderer;

  int x = radius;
  int y = 0;
  int decisionOver2 = 1 - x;

  while(x >= y)
  {
    SDL_RenderDrawLine(rndr,  x + x0,  y + y0,  x + x0, -y + y0);
    SDL_RenderDrawLine(rndr,  y + x0,  x + y0,  y + x0, -x + y0);
    SDL_RenderDrawLine(rndr, -x + x0,  y + y0, -x + x0, -y + y0);
    SDL_RenderDrawLine(rndr, -y + x0,  x + y0, -y + x0, -x + y0);

    y++;

    if (decisionOver2>0) {
      decisionOver2 += ((y - (--x))<<1)+1;
    }
    else {
      decisionOver2 += (y<<1)+1;
    }
  }
  return PL_TRUE;
}


//--------------------------------------------------------------------
//
//                Texture / Surface / Renderer Functions
//
//--------------------------------------------------------------------
PlBool gp_SDL_LoadBMP(char* filename, PlLong *surface)
{
  SDL_Surface *image = SDL_LoadBMP(filename);

  if (!image) {
    return RETURN_SDL_FAIL(SDL_LoadBMP);
  }

  *surface = (PlLong)image;
  return PL_TRUE;
}


PlBool gp_SDL_FreeSurface(PlLong surface)
{
  SDL_FreeSurface((SDL_Surface*)surface);
  return PL_TRUE;
}


PlBool gp_SDL_CreateTextureFromSurface(PlLong rndr, PlLong bitmap, PlLong *texture)
{
  SDL_Texture* tex = SDL_CreateTextureFromSurface(
      (SDL_Renderer*)rndr,
      (SDL_Surface*)bitmap);

  if (!tex) {
    RETURN_SDL_FAIL(SDL_CreateTextureFromSurface);
  }

  *texture = (PlLong)tex;
  return PL_TRUE;
}


PlBool gp_SDL_RenderCopyDefaults(PlLong dstRenderer, PlLong srcTexture)
{
  SDL_RenderCopy(
      (SDL_Renderer*)dstRenderer,
      (SDL_Texture*)srcTexture,
      NULL,
      NULL);

  return PL_TRUE;
}


PlBool gp_SDL_RenderCopy(
    PlLong dstRenderer,
    PlLong dstX, PlLong dstY, PlLong dstW, PlLong dstH,
    PlLong srcTexture,
    PlLong srcX, PlLong srcY, PlLong srcW, PlLong srcH)
{
  SDL_Rect srcRect;
  SDL_Rect dstRect;

  srcRect.x = srcX;
  srcRect.y = srcY;
  srcRect.w = srcW;
  srcRect.h = srcH;

  dstRect.x = dstX;
  dstRect.y = dstY;
  dstRect.w = dstW;
  dstRect.h = dstH;

  SDL_RenderCopy(
      (SDL_Renderer*)dstRenderer,
      (SDL_Texture*)srcTexture,
      &srcRect,
      &dstRect);

  return PL_TRUE;
}


//--------------------------------------------------------------------
//
//                    Display functions
//
//--------------------------------------------------------------------
PlBool gp_SDL_GetNumVideoDisplays(PlLong *displayCount)
{
  int count = SDL_GetNumVideoDisplays();

  if (count >= 1) {
    *displayCount = (PlLong)count;
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetNumVideoDisplays);
}


PlBool gp_SDL_GetDisplayBounds(PlLong display, PlLong *x, PlLong *y, PlLong *w, PlLong *h)
{
  SDL_Rect rect;

  if (0 == SDL_GetDisplayBounds((int)display, &rect)) {
    *x = (PlLong)rect.x;
    *y = (PlLong)rect.y;
    *w = (PlLong)rect.w;
    *h = (PlLong)rect.h;
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetDisplayBounds);
}


// Output term is:
//
//    display_mode( <format>, <width>, <height>, <refresh-rate> ).
//
void displayModeToTerm(SDL_DisplayMode *mode, PlTerm *term)
{
    PlTerm args[] = {
      Pl_Mk_Integer((PlLong)mode->format),
      Pl_Mk_Integer((PlLong)mode->w),
      Pl_Mk_Integer((PlLong)mode->h),
      Pl_Mk_Integer((PlLong)mode->refresh_rate)
    };
    *term = Pl_Mk_Compound(g_atomDisplayMode, 4, &args[0]);
}


PlBool gp_SDL_GetCurrentDisplayMode(int index, PlTerm *mode)
{
  SDL_DisplayMode displayMode;

  if (0 == SDL_GetCurrentDisplayMode((int)index, &displayMode)) {
    displayModeToTerm(&displayMode, mode);
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetCurrentDisplayMode);
}


PlBool gp_SDL_GetDesktopDisplayMode(PlLong index, PlTerm *mode)
{
  SDL_DisplayMode displayMode;

  if (0 == SDL_GetDesktopDisplayMode((int)index, &displayMode)) {
    displayModeToTerm(&displayMode, mode);
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetDesktopDisplayMode);
}


PlBool gp_SDL_GetDisplayMode(PlLong index, PlLong modeIndex, PlTerm *mode)
{
  SDL_DisplayMode displayMode;

  if (0 == SDL_GetDisplayMode((int)index, (int)modeIndex, &displayMode)) {
    displayModeToTerm(&displayMode, mode);
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetDisplayMode);
}


//--------------------------------------------------------------------
//
//                    Window functions
//
//--------------------------------------------------------------------
PlBool gp_SDL_SetWindowFullScreen(PlLong window, PlLong flags)
{
  if (0 == SDL_SetWindowFullscreen((SDL_Window*)window,(Uint32)flags)) {
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_SetWindowFullScreen);
}


PlBool gp_SDL_GetWindowFlags(PlLong window, PlLong * flags)
{
  Uint32 wndFlags = SDL_GetWindowFlags((SDL_Window*)window);
  *flags = (PlLong)wndFlags;
  return PL_TRUE;
}


//--------------------------------------------------------------------
//
//                    Audio functions
//
//--------------------------------------------------------------------
PlBool gp_SDL_GetNumAudioDevices(PlBool isCapture, PlLong *count)
{
  *count = (PlLong)SDL_GetNumAudioDevices((int)isCapture);
  return PL_TRUE;
}

PlBool gp_SDL_GetNumAudioDrivers(PlLong *count)
{
  *count = (PlLong)SDL_GetNumAudioDrivers();
  return PL_TRUE;
}


//--------------------------------------------------------------------
//
//                    Platform functions
//
//--------------------------------------------------------------------
PlBool gp_SDL_GetPlatform(PlTerm *output)
{
  *output = Pl_Mk_Atom(Pl_Create_Atom(SDL_GetPlatform()));
  return  PL_TRUE;
}


//--------------------------------------------------------------------
//
//                    Threading functions
//
//--------------------------------------------------------------------

// if I made the data the PlTerm that was the callback....

int gp_ThreadHandler(void* data)
{
  int functor, arity;
  int result;
  PlTerm* pTerm = (PlTerm*)data;
  PlTerm* arg;

  //  arg = Pl_Rd_Callable_Check(*pTerm, &functor, &arity);
  //Pl_Query_Begin(PL_FALSE);

  //result = Pl_Query_Call(functor, arity, arg);

  //Pl_Query_End(PL_CUT); // release memory?!?!

  Pl_Write(*pTerm);

  free(data);
  fprintf(stdout, "THREAD DEATH\n");
  return result;
}


PlBool gp_SDL_CreateThread(PlTerm  callback,char*   threadName,PlTerm* thread)
{
  PlTerm* pTerm = calloc(1, sizeof(PlTerm));

  if (pTerm)
  {
    Pl_Copy_Term(pTerm, &callback);

    SDL_Thread *t = SDL_CreateThread(gp_ThreadHandler, threadName, (void*)pTerm);

    if (t) {
      SDL_DetachThread(t);
      *thread = (PlLong)t;
      return PL_TRUE;
    }
    RETURN_SDL_FAIL(SDL_CreateThread);
  }
  return PL_FALSE;
}


//--------------------------------------------------------------------
//
//                    Truetype (TFF) Functions
//
//                            "SDL_ttf"
//
// NB: --with-sdl-prefix !
//--------------------------------------------------------------------
PlBool gp_TTF_Init()
{
  if (0 == TTF_Init()) {
    return PL_TRUE;
  }
  RETURN_TTF_FAIL(TTF_Init);
}


PlBool gp_TTF_Quit()
{
  TTF_Quit();
  return PL_TRUE;
}


PlBool gp_TTF_OpenFont(char* fontName, PlLong size, PlTerm *font)
{
  TTF_Font *pFont = TTF_OpenFont(fontName, (int)size);

  if (!pFont) {
    RETURN_TTF_FAIL(TTF_OpenFont);
  }

  *font = (PlLong)pFont;
  return PL_TRUE;
}


PlBool gp_TTF_CloseFont(PlLong font)
{
  TTF_CloseFont((TTF_Font*)font);
  return PL_TRUE;
}


PlBool gp_TTF_RenderUTF8_Solid(PlTerm rndr, PlTerm font, PlLong x, PlLong y, char* text)
{
  SDL_Color color = {255, 255, 255, 255};

  SDL_GetRenderDrawColor(
      (SDL_Renderer*)rndr,
      &color.r, &color.g, &color.b, &color.a);

  renderFont(
      (SDL_Renderer*)rndr,
      TTF_RenderUTF8_Solid((TTF_Font*)font, text, color),
      (int)x, (int)y);

  return PL_TRUE;
}


PlBool gp_TTF_RenderUTF8_Blended(PlTerm rndr, PlTerm font, PlLong x, PlLong y, char* text)
{
  SDL_Color color = {255, 255, 255, 255};

  SDL_GetRenderDrawColor(
      (SDL_Renderer*)rndr,
      &color.r, &color.g, &color.b, &color.a);

  renderFont(
      (SDL_Renderer*)rndr,
      TTF_RenderUTF8_Blended((TTF_Font*)font, text, color),
      (int)x, (int)y);

  return PL_TRUE;
}


PlBool gp_TTF_RenderUTF8_Shaded(PlTerm rndr, PlTerm font, PlLong x, PlLong y, char* text)
{
  SDL_Color color  = {255, 255, 255, 255};
  SDL_Color black  = {0, 0, 0, 255};

  SDL_GetRenderDrawColor(
      (SDL_Renderer*)rndr,
      &color.r, &color.g, &color.b, &color.a);

  renderFont(
      (SDL_Renderer*)rndr,
      TTF_RenderUTF8_Shaded((TTF_Font*)font, text, color, black),
      (int)x, (int)y);
  
  return PL_TRUE;
}


PlBool gp_TTF_SizeUTF8(PlTerm font, char* text, PlLong *width, PlLong *height)
{
  int w, h;

  *width  = 0;
  *height = 0;

  if (TTF_SizeUTF8((TTF_Font*)font, text, &w, &h)) {
    RETURN_TTF_FAIL(TTF_SizeUTF8);
  }

  *width  = (PlLong)w;
  *height = (PlLong)h;

  return PL_TRUE;
}


/**
 * Renders a texture containing a rendered font incarnation to the
 * specified (x,y) location in the output.
 *
 * @param SDL_Renderer* r contains the destination renderer
 * @param SDK_Surface*  s contains the rendered text source data
 * @param int x         the output x-coordinate in "r"
 * @param int y         the output y-coordinate in "r"
 */
void renderFont(SDL_Renderer* r, SDL_Surface* pText, int x, int y)
{
  SDL_Rect  dstRect = {(int)x, (int)y};

  if (pText) {
    SDL_Texture *tex = SDL_CreateTextureFromSurface(r, pText);
    if (tex) {
      SDL_QueryTexture(tex, NULL, NULL, &dstRect.w, &dstRect.h);
      if(SDL_RenderCopy(r, tex, NULL, &dstRect)) {
	SHOW_SDL_FAIL(TTF_RenderUTF8_Solid);
      }
    }
    else {
      SHOW_SDL_FAIL(SDL_CreateTextureFromSurface);
    }
    SDL_FreeSurface(pText);
  }
  else {
    SHOW_TTF_FAIL(TTF_RenderUTF8_Solid);
  }
}


//--------------------------------------------------------------------
//
//                    Mixer functions
//
//                      "SDL_mixer"
//
//--------------------------------------------------------------------
PlBool gp_Mix_Linked_Version(PlLong *major, PlLong *minor, PlLong *patch)
{
  SDL_version compile_version;
  const SDL_version *link_version = Mix_Linked_Version();
  SDL_MIXER_VERSION(&compile_version);  

  *major = (PlLong)compile_version.major;
  *minor = (PlLong)compile_version.minor;
  *patch = (PlLong)compile_version.patch;
  
  return PL_TRUE;
}


PlBool gp_Mix_Init(PlLong flags)
{
  int result = Mix_Init(flags);

  if (result) {
    RETURN_MIX_FAIL(Mix_Init);
  }
  return PL_TRUE;
}


PlBool gp_Mix_Quit()
{
  Mix_Quit();
  return PL_TRUE;
}


PlBool gp_Mix_OpenAudio(
    PlLong frequency, PlLong format,
    PlLong channels, PlLong chunks)
{
  if (0 == format) {
    format = MIX_DEFAULT_FORMAT;
  }
  
  if (Mix_OpenAudio((int)frequency, (int)format, (int)channels, (int)chunks)) {
    RETURN_MIX_FAIL(Mix_OpenAudio);
  }
  return PL_TRUE;
}


PlBool gp_Mix_CloseAudio()
{
  Mix_CloseAudio();
  return PL_TRUE;
}


PlBool gp_Mix_LoadMUS(char *filename, PlLong *handle)
{
  Mix_Music *music = Mix_LoadMUS(filename);

  if (0 == music) {
    RETURN_MIX_FAIL(Mix_LoadMUS);
  }
  *handle = (PlLong)music;
  return PL_TRUE;
}


PlBool gp_Mix_FreeMusic(PlLong *handle)
{
  Mix_FreeMusic((Mix_Music*)handle);
  return PL_TRUE;
}


PlBool gp_Mix_PlayMusic(PlLong handle, PlLong loops)
{
  if (-1 == Mix_PlayMusic((Mix_Music*)handle, (int)loops)) {
    RETURN_MIX_FAIL(Mix_PlayMusic);
  }
  return PL_TRUE;
}


PlBool gp_Mix_FadeInMusic(PlLong handle, PlLong loops, PlLong ms)
{
  if (-1 == Mix_FadeInMusic((Mix_Music*)handle, (int)loops, (int)ms)) {
    RETURN_MIX_FAIL(Mix_FadeInMusic);
  }
  return PL_TRUE;
}


PlBool gp_Mix_FadeInMusicPos(PlLong handle, PlLong loops, PlLong ms, double pos)
{
  if (-1 == Mix_FadeInMusicPos((Mix_Music*)handle, (int)loops, (int)ms, pos)) {
    RETURN_MIX_FAIL(Mix_FadeInMusicPos);
  }
  return PL_TRUE;
}


PlBool gp_Mix_VolumeMusic(PlLong volume, PlLong *oldVolume)
{
  *oldVolume = (PlLong)Mix_VolumeMusic((int)volume);
  return PL_TRUE;
}


PlBool gp_Mix_HaltMusic()
{
  Mix_HaltMusic();
  return PL_TRUE;
}


PlBool gp_Mix_PauseMusic()
{
  Mix_PauseMusic();
  return PL_TRUE;
}

PlBool gp_Mix_ResumeMusic()
{
  Mix_ResumeMusic();
  return PL_TRUE;
}

PlBool gp_Mix_RewindMusic()
{
  Mix_RewindMusic();
  return PL_TRUE;
}


PlBool gp_Mix_QuerySpec(PlLong *frequency, PlLong *format, PlLong *channels, PlLong *opened)
{
  int hz, chn;
  Uint16 fmt;

  int open = Mix_QuerySpec(&hz, &fmt, &chn);

  if (0 == open) {
    RETURN_MIX_FAIL(Mix_QuerySpec);
  }

  *frequency = (PlLong)hz;
  *format    = (PlLong)fmt;
  *channels  = (PlLong)chn;
  *opened    = (PlLong)open;
  return PL_TRUE;
}
