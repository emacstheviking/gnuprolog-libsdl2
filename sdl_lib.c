#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <gprolog.h>
#include <SDL.h>

#include "sdllib_common.h"
#undef __DEBUG__


// These are created during SDL_Init to avoid repetition
int g_atomEmptyList      = 0;
int g_atomTextInput      = 0;
int g_atomTextEditing    = 0;
int g_atomDropFile       = 0;
int g_atomDisplayMode    = 0;
int g_atomUserEvent      = 0;
int g_atomBlendModeNone  = 0;
int g_atomBlendModeBlend = 0;
int g_atomBlendModeAdd   = 0;
int g_atomBlendModeMod   = 0;
int g_atomDollarRecord   = 0;
int g_atomDollarGesture  = 0;

// stderr for SDL_Log can be redirected to a file
FILE* g_errFile  = NULL;
FILE* g_oldStdErr;

//typedef void (*SDL_LogOutputFunction)(void *userdata, int category, SDL_LogPriority priority, const char *message);
	   

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
EVWRAPPER_TERM(evUserEvent);
EVWRAPPER_TERM(evDollarEvent);

// Maps event codes into strings for atom conversion
const char* evWindowType(int);
const char* evButtonName(Uint32);




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
  // input event types
  g_atomTextInput      = Pl_Create_Atom("text_input");
  g_atomTextEditing    = Pl_Create_Atom("text_editing");
  g_atomDropFile       = Pl_Create_Atom("dropfile");
  g_atomDisplayMode    = Pl_Create_Atom("display_mode");
  g_atomUserEvent      = Pl_Create_Atom("user_event");
  // texture blending modes
  g_atomBlendModeNone  = Pl_Create_Atom("none");
  g_atomBlendModeBlend = Pl_Create_Atom("blend");
  g_atomBlendModeAdd   = Pl_Create_Atom("add");
  g_atomBlendModeMod   = Pl_Create_Atom("mod");
  // gesture events
  g_atomDollarRecord   = Pl_Create_Atom("dollar_record");
  g_atomDollarGesture  = Pl_Create_Atom("dollar_gesture");
  // generic
  g_atomEmptyList      = Pl_Create_Atom("[]");


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

#ifdef __DEBUG__
    fprintf(stdout, "gp_SDL_CreateWindow: ok: %p\n", wnd);
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


PlBool gp_SDL_ShowWindow(PlLong handle)
{
  SDL_ShowWindow((SDL_Window*)handle);
  return PL_TRUE;
}


PlBool gp_SDL_HideWindow(PlLong handle)
{
  SDL_HideWindow((SDL_Window*)handle);
  return PL_TRUE;
}


PlBool gp_SDL_RaiseWindow(PlLong handle)
{
  SDL_RaiseWindow((SDL_Window*)handle);
  return PL_TRUE;
}


PlBool gp_SDL_SetWindowTitle(PlLong wnd, char* title)
{
    SDL_SetWindowTitle((SDL_Window*)wnd, title);
    return PL_TRUE;
}


PlBool gp_SDL_SetWindowSize(PlLong wnd, PlLong width, PlLong height)
{
  SDL_SetWindowSize((SDL_Window*)wnd, (int)width, (int)height);
  return PL_TRUE;
}


PlBool gp_SDL_GetWindowSize(PlLong wnd, PlLong *width, PlLong *height)
{
  int ww, wh;
  SDL_GetWindowSize((SDL_Window*)wnd, &ww, &wh);
  *width  = (PlLong)ww;
  *height = (PlLong)wh;
  return PL_TRUE;
}


PlBool gp_SDL_SetWindowPosition(PlLong wnd, PlLong x, PlLong y)
{
  SDL_SetWindowPosition((SDL_Window*)wnd, (int)x, (int)y);
  return PL_TRUE;
}


PlBool gp_SDL_GetWindowPosition(PlLong wnd, PlLong *x, PlLong *y)
{
  int wx, wy;
  SDL_GetWindowPosition((SDL_Window*)wnd, &wx, &wy);
  *x = (PlLong)wx;
  *y = (PlLong)wy;
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


PlBool gp_SDL_GetRenderer(PlLong window, PlLong *rndr)
{
  SDL_Renderer *r = SDL_GetRenderer((SDL_Window*)window);

  if (r) {
    *rndr = (PlLong)r;
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetRenderer);
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


//TODO: make a function that returns a list
//of all the gathered events in C, so that we
//dont get needless marshaling between prolog and C
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

	case SDL_DOLLARGESTURE:
	case SDL_DOLLARRECORD:     EVB_TERM(evDollarEvent);

	case SDL_QUIT:             EVB(evQuit);
	case SDL_DROPFILE:         EVB_TERM(evDropEvent);

	case SDL_USEREVENT:        EVB_TERM(evUserEvent);

	default:
	  //sprintf(szTerm, "unhandled(%u)", ev.type);
	  szTerm[0] = (char)0;
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


PlBool gp_SDL_Delay(PlLong delay_in_milliseconds)
{
  SDL_Delay(delay_in_milliseconds);
  return PL_TRUE;
}


PlBool gp_SDL_RemoveTimer(PlLong timer)
{
  if (SDL_RemoveTimer((SDL_TimerID)timer)) {
    return PL_TRUE;
  }
  return PL_FALSE;
}


Uint32 timerCallback(Uint32 interval, void* param)
{
  SDL_Event event;

  event.type       = SDL_USEREVENT;
  event.user.code  = interval;
  event.user.data1 = 0;
  event.user.data2 = 0;

  int status = SDL_PushEvent(&event);

#ifdef __DEBUG__
  switch(status) {
      case 1: fprintf(stdout, "timer+user-event => pushed ok\n"); break;
      case 0: fprintf(stdout, "timer+user-event => filtered!\n"); break;
      default:
	SHOW_SDL_FAIL(SDL_PushEvent);
	break;
  }
#endif

  // abort subsequent callbacks unless success
  return (status < 0) ? 0 : interval;
}


PlBool gp_SDL_AddTimer(PlLong interval, PlLong *timerId)
{
  SDL_TimerID id = SDL_AddTimer(interval, timerCallback, NULL);

  if (id) {
    *timerId = (PlLong)id;
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_AddTimer);
}



/**
 * SDL_ShowSimpleMessageBox
 *
 * @param SDL_Window* parent  the parent window, 0 for no parent.
 * @param PlLong      flags   simple message box flags
 * @param char*       title   C-string title of the message
 * @param char*       message C-string content of the message
 *
 * @return PL_TRUE
 */
PlBool gp_SDL_ShowSimpleMessageBox_C(
    PlLong parent, char* title,
    char* message, PlLong flags)
{
  SDL_ShowSimpleMessageBox(flags, title, message, (SDL_Window*)parent);
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


// pass "" to restore output to whatver is the default
PlBool gp_SDL_LogToFile(char* filename)
{
  if (strlen(filename)) {
    if (!g_errFile) {
      g_oldStdErr = stderr;
      g_errFile = freopen(filename, "a+", stderr);
      if (NULL == g_errFile) {
	fprintf(stderr, "Failed to redirect: %i\n", errno);
	return PL_FALSE;
      }
    }
  }
  else {
    fclose(stderr);
    stderr = g_oldStdErr;
    g_oldStdErr = NULL;
    g_errFile = NULL;
  }
  return PL_TRUE;
}


PlBool gp_SDL_Log(char* text)
{
  SDL_Log("%s", text);
  return PL_TRUE;
}


PlBool gp_SDL_LogMessage(PlLong category, PlLong priority, char* text)
{
  SDL_LogMessage((int)category, (int)priority, "%s", text);
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
	  "%s(%u,%u,%i,%s,%s,%i,%i,%i)",
	  e->type == SDL_MOUSEBUTTONDOWN ? "mouse_down" : "mouse_up",
	  e->button.timestamp,
	  e->button.windowID,
	  e->button.which,
	  evButtonName(e->button.button),
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
// user_event( <timestamp>, <windowID>, <code>, <positive>, <positive>).
//--------------------------------------------------------------------
EVWRAPPER_TERM(evUserEvent)
{
  PlTerm args[] = {
    Pl_Mk_Integer((PlLong)e->user.timestamp),
    Pl_Mk_Integer((PlLong)e->user.windowID),
    Pl_Mk_Integer(e->user.code),
    Pl_Mk_Positive((PlLong)e->user.data1),
    Pl_Mk_Positive((PlLong)e->user.data2)
  };

  *t = 0;
  *o = Pl_Mk_Compound(g_atomUserEvent, 5, &args[0]);
}


//--------------------------------------------------------------------
// dollar_gesture/event( <timestamp>, <windowID>, <code>, <positive>, <positive>).
//--------------------------------------------------------------------
EVWRAPPER_TERM(evDollarEvent)
{
  int dollarEventType = (e->type == SDL_DOLLARGESTURE)
    ? g_atomDollarGesture
    : g_atomDollarRecord;

#ifdef __DEBUG__
  fprintf(stdout, "DOLLARx: type: %i\n", e->type);
#endif
  
  PlTerm args[] = {
    Pl_Mk_Integer((PlLong)e->dgesture.timestamp),
    Pl_Mk_Integer((PlLong)e->dgesture.touchId),
    Pl_Mk_Integer((PlLong)e->dgesture.gestureId),
    Pl_Mk_Positive((PlLong)e->dgesture.numFingers),
    Pl_Mk_Float((double)e->dgesture.error),
    Pl_Mk_Float((double)e->dgesture.x),
    Pl_Mk_Float((double)e->dgesture.y)
  };

  *t = 0;
  *o = Pl_Mk_Compound(dollarEventType, 7, &args[0]);
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
// Mouse Event -> Descriptive button name for functor use.
//--------------------------------------------------------------------
const char* evButtonName(Uint32 id) {
  switch(id) {
      case SDL_BUTTON_LEFT: return "left";
      case SDL_BUTTON_MIDDLE: return "middle";
      case SDL_BUTTON_RIGHT: return "right";
      case SDL_BUTTON_X1: return "x1";
      case SDL_BUTTON_X2: return "x2";
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


PlBool gp_SDL_RenderCopyEx(
    PlLong dstRenderer,
    PlLong dstX, PlLong dstY, PlLong dstW, PlLong dstH,
    PlLong srcTexture,
    PlLong srcX, PlLong srcY, PlLong srcW, PlLong srcH,
    double angle,
    PlLong centerX, PlLong centerY,
    PlLong flip)
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

PlBool gp_SDL_FlushEvents(PlLong mintype, PlLong maxtype)
{
	SDL_FlushEvents((uint32_t) mintype, (uint32_t) maxtype);
	return PL_TRUE;
}

PlBool gp_SDL_FlushEvent(PlLong type)
{
	SDL_FlushEvent((uint32_t) type);
	return PL_TRUE;
}

PlBool gp_SDL_PumpEvents(void)
{
	SDL_PumpEvents();
	return PL_TRUE;
}

PlBool gp_SDL_EventState(PlLong type, PlLong state)
{
	SDL_EventState((uint32_t) type, (int) state);
	return PL_TRUE;
}

PlBool gp_SDL_QueryTexture(
    SDL_Texture *texture,
    PlLong *format, PlLong* access,
    PlLong *width,  PlLong *height)
{
  Uint32 f;
  int a, w, h;
  
  if (0 != SDL_QueryTexture((SDL_Texture*)texture, &f, &a, &w, &h)) {
    RETURN_SDL_FAIL(SDL_QueryTexture);
  }

  *format = (PlLong)f;
  *access = (PlLong)a;
  *width  = (PlLong)w;
  *height = (PlLong)h;
  return PL_TRUE;
}


PlBool gp_SDL_GetTextureAlphaMod(PlLong texture, PlLong *alpha)
{
  Uint8 al;

  if (0 == SDL_GetTextureAlphaMod((SDL_Texture*)texture, &al)) {
    *alpha = (PlLong)al;
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetTextureAlphaMod);
}


PlBool gp_SDL_SetTextureAlphaMod(PlLong texture, PlLong alpha)
{
  if (0 == SDL_SetTextureAlphaMod((SDL_Texture*)texture, (Uint8)alpha)) {
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_SetTextureAlphaMod);
}


PlBool gp_SDL_GetTextureBlendMode(PlLong texture, PlTerm *mode)
{
  SDL_BlendMode bmode;
  
  if (0 == SDL_GetTextureBlendMode((SDL_Texture*)texture, &bmode)) {
    switch(bmode) {
	case SDL_BLENDMODE_NONE:  *mode = g_atomBlendModeNone;  break;
	case SDL_BLENDMODE_BLEND: *mode = g_atomBlendModeBlend; break;
	case SDL_BLENDMODE_ADD:   *mode = g_atomBlendModeAdd;   break;
	case SDL_BLENDMODE_MOD:   *mode = g_atomBlendModeMod;   break;
    }
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetTextureBlendMode);
}


PlBool gp_SDL_SetTextureBlendMode(PlLong texture, char* mode)
{
  SDL_BlendMode bmode  = SDL_BLENDMODE_NONE;
  if (!strcpy("blend", mode)) {
    bmode = SDL_BLENDMODE_BLEND;
  }
  else if (!strcpy("add", mode)) {
    bmode = SDL_BLENDMODE_ADD;
  }
  else if (!strcpy("mod", mode)) {
    bmode = SDL_BLENDMODE_MOD;
  }
  
  if (0 == SDL_SetTextureBlendMode((SDL_Texture*)texture, (SDL_BlendMode)mode)) {
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_SetTextureBlendMode);
}


PlBool gp_SDL_GetTextureColorMod(PlLong texture, PlLong *red, PlLong *grn, PlLong *blu)
{
  Uint8 r, g, b;
  if (0 == SDL_GetTextureColorMod((SDL_Texture*)texture, &r, &g, &b)) {
    *red = (PlLong)r;
    *grn = (PlLong)g;
    *blu = (PlLong)b;
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetTextureColorMod);
}


PlBool gp_SDL_SetTextureColorMod(PlLong texture, PlLong red, PlLong grn, PlLong blu)
{
  if (0 == SDL_SetTextureColorMod((SDL_Texture*)texture, (Uint8)red, (Uint8)grn, (Uint8)blu)) {
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_SetTextureColorMod);
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
//                    Clipboard functions
//
//--------------------------------------------------------------------
PlBool gp_SDL_GetClipboardText(PlTerm *text)
{
  char* cbtext = SDL_GetClipboardText();

  if (NULL != cbtext) {
    *text = Pl_Mk_Codes((const char*)cbtext);
  }
  else {
    *text = g_atomEmptyList;
  }
  return PL_TRUE;
}


PlBool gp_SDL_SetClipboardText(char* cbtext)
{
  if (0 == SDL_SetClipboardText((const char*)cbtext)) {
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_SetClipboardText);
}


PlBool gp_SDL_HasClipboardText()
{
  if (SDL_HasClipboardText()) {
    return PL_TRUE;
  }
  return PL_FALSE;
}


//--------------------------------------------------------------------
//
//      Touch devices & Gesture (DOLLAR) recognition functions
//
//--------------------------------------------------------------------
PlBool gp_SDL_GetNumTouchDevices(PlLong *count)
{
  *count = (PlLong)SDL_GetNumTouchDevices();
  return PL_TRUE;
}


PlBool gp_SDL_RecordGesture(PlLong touchId)
{
  int result = SDL_RecordGesture(-1);
  fprintf(stdout, "SDL_RecordGesture result: %i\n", result);

  if (result < 0) {
    fprintf(stdout, "Failed to start gesture recording :(\n");
    RETURN_SDL_FAIL(SDL_RecordGesture);
  }

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


PlBool gp_SDL_GetAudioDriver(PlLong index, PlTerm *name)
{
  const char* driver = SDL_GetAudioDriver((int)index);

  if (NULL != driver) {
    *name = Pl_Create_Atom(driver);
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetAudioDriver);
}


PlBool gp_SDL_GetAudioDeviceName(PlLong index, PlLong iscapture, PlTerm *name)
{
  const char *device = SDL_GetAudioDeviceName((int)index, (int)iscapture);

  if (device) {
    *name = Pl_Create_Atom(device);
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_GetAudioDeviceName);
}


PlBool gp_SDL_RenderGetScale(PlLong rndr, double *scaleX, double *scaleY)
{
  float sx, sy;
  SDL_RenderGetScale((SDL_Renderer*)rndr, &sx, &sy);

  *scaleX = (double)sx;
  *scaleY = (double)sy;

  return PL_TRUE;
}


PlBool gp_SDL_RenderSetScale(PlLong rndr, double scaleX, double scaleY)
{
  if (0 == SDL_RenderSetScale((SDL_Renderer*)rndr, (float)scaleX, (float)scaleY)) {
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_RenderSetScale);
}


PlBool gp_SDL_RenderGetLogicalSize(PlLong rndr, PlLong *width, PlLong *height)
{
  int w, h;

  SDL_RenderGetLogicalSize((SDL_Renderer*)rndr, &w, &h);

  *width  = (PlLong)w;
  *height = (PlLong)h;

  return PL_TRUE;
}


PlBool gp_SDL_RenderSetLogicalSize(PlLong rndr, PlLong width, PlLong height)
{
  int w, h;
  SDL_RenderGetLogicalSize((SDL_Renderer*)rndr, &w, &h);

  return PL_TRUE;
}


PlBool gp_SDL_RenderGetViewport(PlLong rndr, PlLong *x, PlLong *y, PlLong *w, PlLong *h)
{
  SDL_Rect rect;
  SDL_RenderGetViewport((SDL_Renderer*)rndr, &rect);

  *x = (PlLong)rect.x;
  *y = (PlLong)rect.y;
  *w = (PlLong)rect.w;
  *h = (PlLong)rect.h;

  return PL_TRUE;
}


PlBool gp_SDL_RenderGetClipRect(PlLong rndr, PlLong *x, PlLong *y, PlLong *w, PlLong *h)
{
  SDL_Rect rect;
  SDL_RenderGetClipRect((SDL_Renderer*)rndr, &rect);

  *x = (PlLong)rect.x;
  *y = (PlLong)rect.y;
  *w = (PlLong)rect.w;
  *h = (PlLong)rect.h;

  return PL_TRUE;
}


PlBool gp_SDL_RenderSetClipRect(PlLong rndr, PlLong x, PlLong y, PlLong w, PlLong h)
{
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  
  if(0 == SDL_RenderSetClipRect((SDL_Renderer*)rndr, &rect)) {
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_RenderSetClipRect);
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
