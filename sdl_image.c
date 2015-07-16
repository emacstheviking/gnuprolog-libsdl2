#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gprolog.h>
#include <SDL.h>
#include <SDL_image.h>

#include "sdllib_common.h"


#define RETURN_IMG_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, IMG_GetError());	\
  return PL_FALSE;


#define SHOW_IMG_FAIL(F) fprintf(stderr, "%s failed -> %s\n", #F, IMG_GetError());


//--------------------------------------------------------------------
//
//                    Image functions
//
//                      "SDL_image"
//
//--------------------------------------------------------------------
PlBool gp_IMG_Linked_Version(PlLong *major, PlLong *minor, PlLong *patch)
{
  SDL_version compile_version;
  const SDL_version *link_version = IMG_Linked_Version();
  SDL_IMAGE_VERSION(&compile_version);  

  *major = (PlLong)compile_version.major;
  *minor = (PlLong)compile_version.minor;
  *patch = (PlLong)compile_version.patch;
  
  return PL_TRUE;
}


PlBool gp_IMG_Init(PlLong flags, PlLong *loaded)
{
  int initted = IMG_Init(flags);
#ifdef __DEBUG__
  fprintf(stdout, "IMG_Init has done: %02x\n", initted);
#endif
  
  if ((initted & flags) != flags) {
    RETURN_IMG_FAIL(IMG_Init);
  }
  *loaded = (PlLong)initted;
  return PL_TRUE;
}


PlBool gp_IMG_Quit()
{
  IMG_Quit();
  return PL_TRUE;
}


PlBool gp_IMG_Load(char* filename, PlLong *handle)
{
  SDL_Surface *image = IMG_Load(filename);

  fprintf(stdout, "%p\n", image);

  if (image) {
    *handle = (PlLong)image;
    return PL_TRUE;
  }

  RETURN_IMG_FAIL(IMG_Load);
}
