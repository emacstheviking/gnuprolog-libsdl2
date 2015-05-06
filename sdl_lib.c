#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gprolog.h>
#include <SDL2/SDL.h>


// gplc -C -O -L -lsdl2 -o sdltest sdl_lib.pl sdl_lib.c

#define RETURN_SDL_FAIL(F) \
  fprintf(stderr, "%s failed -> %s\n", #F, SDL_GetError()); \
  return PL_FALSE;

#define IS_NIL(X)  Pl_Un_Atom_Check(Pl_Atom_Nil(), X)
#define NOT_NIL(X) !IS_NIL(X)




/*
 *--------------------------------------------------------------------
 *
 * SDL_Init
 *
 *--------------------------------------------------------------------
 */

PlBool
gp_sdl_init(PlTerm list_of_atoms)
{
  int nFlags = Pl_List_Length(list_of_atoms);

  if (nFlags)
    {
      PlTerm *cells = calloc(nFlags, sizeof(PlTerm));
      int nActual = Pl_Rd_Proper_List_Check(list_of_atoms, cells);

      if (nActual == nFlags)
	{
	  int i, atom;

	  for(i = 0; i < nActual; i++ )
	    {
	      if ((atom = Pl_Rd_Atom_Check(cells[i])))
		{
		  fprintf(stderr, "SDL flag: %s\n", Pl_Atom_Name(atom));
		}
	    }
	}

      free((void*)cells);
      return PL_TRUE;
    }

  RETURN_SDL_FAIL(SDL_Init);
}




/*
 *--------------------------------------------------------------------
 *
 * SDL_Quit
 *
 *--------------------------------------------------------------------
 */

PlBool
gp_sdl_quit()
{
  SDL_Quit();

  return PL_TRUE;
}
