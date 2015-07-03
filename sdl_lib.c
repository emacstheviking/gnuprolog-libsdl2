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
  SDL_Window *wnd = (SDL_Window*)handle;

  SDL_DestroyWindow(wnd);

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
 * @param PlLong handle  references the window to be killed
 *
 * @return PL_TRUE
 *
 */
PlBool gp_SDL_DestroyRenderer(PlLong handle)
{
  SDL_Window *wnd = (SDL_Window*)handle;

  SDL_DestroyWindow(wnd);

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


 /* PlBool term_atom_or_positive(PlTerm x, char** keys, Uint32* ) */
/* { */
/*   int atom; */
/*   Uint32 value; */
/*   PlLong lValue; */
  
/*   switch(Pl_Type_Of_Term(x)) */
/*   { */
/*   case PL_ATM: */
/*     atom = Pl_Rd_Atom(x); */
/*     value = map_lookup(Pl_Atom_Name(atom), gstr_CwPos, gbit_CwPos); */

/*     if (0 == value) { */
/*       Pl_Set_C_Bip_Name("sdl_createwindow", 7); */
/*       Pl_Err_Syntax(Pl_Create_Atom("undefined|centered|<integer> expected")); */
/*     } */

/*     DHEX("pos", value); */
/*     break; */

/*   case PL_INT: */
/*     lValue = Pl_Rd_Positive(x); */
/*     DULONG("arse:positive", value); */
/*     break; */

/*   default: */
/*     // throw an error: incorrect type */
/*     Pl_Set_C_Bip_Name("sdl_createwindow", 7); */
/*     Pl_Err_Syntax(Pl_Create_Atom("atom or integer expected")); */
/*     break; */
/*   } */
  
/*   return PL_TRUE; */
/* } */

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
/*
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
*/


/**
 * Lookup a single key from a map table.
 *
 * @param const char* flag_name is the name to be searched for
 * @param char**      keys      contains the key lookup values
 * @param Uint32*     flags     contains corresponding bit values
 *
 * @return Uint32 combined bit flags for any matched keys
 */
/*
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
*/
