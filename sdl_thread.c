#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gprolog.h>
#include <SDL.h>

#include "sdllib_common.h"


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
