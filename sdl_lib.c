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


/**
 * Helper macro for SDL error return from predicate.
 */
#define RETURN_SDL_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, SDL_GetError());	\
  return PL_FALSE;




/**
 * Lookup a single key from a map table.
 *
 * @param const char* flag_name is the name to be searched for
 * @param char**      keys      contains the key lookup values
 * @param Uint32*     flags     contains corresponding bit values
 *
 * @return Uint32 combined bit flags for any matched keys
 */
Uint32 map_lookup(const char* flag_name, char** keys, Uint32* flags)
{
  while(*keys)
  {
    if (!strcmp(flag_name, *keys))
    {

#ifdef __DEBUG__
      fprintf(stderr, "value_map %s => %i\n", flag_name, *flags);
#endif
      return *flags;
    }
    keys++;
    flags++;
  }
  return 0;
}




/**
 * Scans a list of atoms from the call site and for every match in the
 * given string table it build a bit mask from the corresponding value
 * mask.
 *
 * The input is expected to be a list of atoms, any non-atom terms are
 * skipped with no ill effects. An example usage, initialising the SDL
 * library:
 *
 *     sdl_init([video, audio, timer]).
 *
 * @param PlTerm   atom_list   contains the call-site list of atoms
 * @param char**   keys        a key table list from static data file
 * @param Uint32*  flags       corresponding bit flags for keys
 *
 * @return Uint32 combined bit flag value
 */
Uint32 scan_map(PlTerm atom_list, char** keys, Uint32* flags)
{
  int nFlags = Pl_List_Length(atom_list);
  Uint32 result = 0;

  if (nFlags)
  {
    // TODO: if Pl_xxx fails we have leaked this memory. What does
    // the list say is the best way to deal with it.
    PlTerm *cells  = calloc(nFlags, sizeof(PlTerm));
    int    nActual = Pl_Rd_Proper_List_Check(atom_list, cells);
    int    i, atom;

    if (nActual == nFlags)
    {
      for(i = 0; i < nActual; i++ )
      {
	if (Pl_Builtin_Atom(cells[i]))
	{
	  if ((atom = Pl_Rd_Atom(cells[i])))
	  {
	    result |= map_lookup(Pl_Atom_Name(atom),
				 gstr_InitModes,
				 gbit_InitModes);
	  }
	}
      }
    }
    free((void*)cells);
  }

#ifdef __DEBUG__
  fprintf(stderr, "scan_map: return: %08x\n", result);
#endif

  return result;
}



/**
 * Perform standard SDL library initialisation.
 *
 *  flag_list is assumed to be a list of atoms corresponding to the
 *  SDL_INIT_XXX flag. If you pass in junk it will just be ignored. If
 *  at least one bit flag is given then the function should complete
 *  successfully.
 *
 *  atoms recognised:
 *
 *    timer, audio, video, init_joystick, haptic
 *    gamecontroller, events, everything, parachute
 *
 * @param PlTerm flag_list
 *
 * @return PL_TRUE | PL_FALSE
 *
 * @see https://wiki.libsdl.org/SDL_Init
 *--------------------------------------------------------------------
 */
PlBool gp_sdl_init(PlTerm flag_list)
{
  Uint32 flags = scan_map(flag_list, gstr_InitModes, gbit_InitModes);

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
 * @see https://wiki.libsdl.org/SDL_Quit
 *--------------------------------------------------------------------
 */
PlBool gp_sdl_quit()
{
  SDL_Quit();

  return PL_TRUE;
}




/**
 * Create a window of a given size, position and attributes.
 *
 *    sdl_createwindow(
 *        "Hello World",
 *        centered, undefined,
 *        800, 600,
 *        [opengl, allow_highdpi],
 *        Wnd).
 *
 * @param char*  title   atom or string containing the window title
 * @param PlTerm x       centered | undefined | <integer-value>
 * @param PlTerm y       centered | undefined | <integer-value>
 * @param PlLong w       width of the window
 * @param PlLong h       height of the window
 * @param PlTerm flags   one or more SDL_WindowFlags
 * @param PlTerm handle  an UNINSTANTIATED variable to hold the handle
 *
 * @return PL_TRUE | PL_FALSE
 *
 * @see https://wiki.libsdl.org/SDL_CreateWindow
 */
PlBool gp_createwindow(char*  title,
		       PlTerm xpos,
		       PlTerm ypos,
		       PlLong width,
		       PlLong height,
		       PlTerm flag_list,
		       PlLong *handle)
{
  Uint32 flags = scan_map(flag_list, gstr_CwModes, gbit_CwModes);
  SDL_Window* wnd = SDL_CreateWindow(title, 100, 100, 640, 480, 0);

  if (wnd)
  {
    *handle = (PlLong)wnd;
    return PL_TRUE;
  }
  RETURN_SDL_FAIL(SDL_CreateWindow);
}




/**
 * Destroy a window.
 *
 * @param ??? handle  references the window to be killed
 *
 * @return PL_TRUE
 *
 * @see https://wiki.libsdl.org/SDL_DestroyWindow
 */
PlBool gp_destroywindow(PlLong handle)
{
  SDL_Window *wnd = (SDL_Window*)handle;

  SDL_DestroyWindow(wnd);

  return PL_TRUE;
}




/**
 * Delay the process for a number of milliseconds.
 *
 * @param  PlLong   the required delay
 *
 * @return PL_TRUE
 *
 * @see https://wiki.libsdl.org/SDL_Delay
 */
PlBool gp_delay(PlLong delay_in_milliseconds)
{
  SDL_Delay(delay_in_milliseconds);

  return PL_TRUE;
}
