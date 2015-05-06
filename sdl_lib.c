#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gprolog.h>
#include <SDL2/SDL.h>

#include "static_data.h"


// Uncomment for useful output when tinkering
#define __DEBUG__


// gplc -C -O -L -lsdl2 -o sdltest sdl_lib.pl sdl_lib.c


#define RETURN_SDL_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, SDL_GetError());	\
  return PL_FALSE;


//#define IS_NIL(X)  Pl_Un_Atom_Check(Pl_Atom_Nil(), X)
//#define NOT_NIL(X) !IS_NIL(X)




// TODO: might be re-usable code: lookup value from table by string
// the keys array MUST be NULL terminated as its final entry

Uint32 value_map(const char* flag_name, char** keys, Uint32* flags)
{
  while(*keys) {
    if (!strcmp(flag_name, *keys)) {

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




/*
 *--------------------------------------------------------------------
 *
 * scan_map
 *
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
 *--------------------------------------------------------------------
 */
Uint32 scan_map(PlTerm atom_list, char** keys, Uint32* flags)
{
  int nFlags = Pl_List_Length(atom_list);
  Uint32 result = 0;

  if (nFlags) {

    // TODO: if Pl_xxx fails we have leaked this memory. What does
    // the list say is the best way to deal with it.

    PlTerm *cells  = calloc(nFlags, sizeof(PlTerm));
    int    nActual = Pl_Rd_Proper_List_Check(atom_list, cells);
    int    i, atom;

    if (nActual == nFlags)
    {
      for(i = 0; i < nActual; i++ ) {

	if (Pl_Builtin_Atom(cells[i])) {

	  if ((atom = Pl_Rd_Atom(cells[i]))) {

	    result |= value_map(Pl_Atom_Name(atom),
				  gstr_InitModes,
				  gbit_InitModes);
	  }
	}
      }
    }
    free((void*)cells);
  }

#ifdef __DEBUG__
  fprintf(stderr, "scan_map: return: %04x\n", result);
#endif

  return result;
}



/*
 *--------------------------------------------------------------------
 *
 * SDL_Init([flag, flag, ...])
 *
 *  where flag can be one of these atoms corresponding to the
 *  SDL_INIT_XXX flag. If you pass in junk it will just be ignored. If
 *  at least one bit flag is given then the function should complete
 *  successfully.
 *
 *  - timer
 *  - audio
 *  - video
 *  - init_joystick
 *  - haptic
 *  - gamecontroller
 *  - events
 *  - everything
 *  - parachute
 *
 * @see https://wiki.libsdl.org/SDL_Init
 *--------------------------------------------------------------------
 */
PlBool gp_sdl_init(PlTerm flag_list)
{
  Uint32 flags = scan_map(flag_list, gstr_InitModes, gbit_InitModes);

  if (flags > 0) {

    fprintf(stderr, "SDL flag: %08X\n", flags);
    
    if (0 == SDL_Init(flags)) {
      return PL_TRUE;
    }
  }

  RETURN_SDL_FAIL(SDL_Init);
}


/*
 *--------------------------------------------------------------------
 *
 * SDL_Quit => sdl_quit.
 *
 *--------------------------------------------------------------------
 */
PlBool gp_sdl_quit()
{
  SDL_Quit();

  return PL_TRUE;
}


/*
 *--------------------------------------------------------------------
 *
 * SDL_CreateWindow => sdl_createwindow(title, x, y, w, h, flags)
 *
 * title => atom or string containing the window title
 * x, y  => centered | undefined | <integer-value>
 * w, h  => width and height of the window
 * flags => one or more SDL_WindowFlags
 *
 
   sdl_createwindow(
     "Hello World",
     centered, undefined,
     800, 600,
     [opengl, allow_highdpi],
     Wnd).

 *--------------------------------------------------------------------
 */
PlBool gp_createwindow(PlTerm title,
		       PlTerm xpos,
		       PlTerm ypos,
		       PlLong width,
		       PlLong height,
		       PlTerm flag_list,
		       PlTerm handle)
{
  Uint32 flags = scan_map(flag_list, gstr_CwModes, gbit_CwModes);

  fprintf(stderr, "WND flag: %08X\n", flags);  

  return PL_FALSE;
}
