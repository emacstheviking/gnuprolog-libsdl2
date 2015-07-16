#define RETURN_SDL_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, SDL_GetError());	\
  return PL_FALSE;

#define SHOW_SDL_FAIL(F) fprintf(stderr, "%s failed -> %s\n", #F, SDL_GetError());


// Uncomment for useful output when tinkering
// #define __DEBUG__


// Debugging helpers
#define DSTR(L,X) fprintf(stdout, "%s => %s\n", L, X);
#define DHEX(L,X) fprintf(stdout, "%s => %08X\n", L, (unsigned int)X);
#define DULONG(L,X) fprintf(stdout, "%s => %lu\n", L, (unsigned long)X);
