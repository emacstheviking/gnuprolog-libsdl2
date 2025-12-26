#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gprolog.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "sdllib_common.h"


#define RETURN_TTF_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, TTF_GetError());	\
  return PL_FALSE;

#define SHOW_TTF_FAIL(F) fprintf(stderr, "%s failed -> %s\n", #F, TTF_GetError());


// render a font texture into a renderer
void renderFont(SDL_Renderer* r, SDL_Surface* pText, int x, int y);




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
	SHOW_TTF_FAIL(TTF_RenderUTF8_Solid);
      }
	  SDL_DestroyTexture(tex);
    }
    else {
      SHOW_TTF_FAIL(SDL_CreateTextureFromSurface);
    }
    SDL_FreeSurface(pText);
  }
  else {
    SHOW_TTF_FAIL(TTF_RenderUTF8_Solid);
  }
}
