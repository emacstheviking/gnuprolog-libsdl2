%%====================================================================
%%
%%                            FFI DEFINITIONS
%%
%%====================================================================

:- foreign(sdl_Init_C(+positive), [fct_name(gp_SDL_Init)]).
:- foreign(sdl_Quit,              [fct_name(gp_SDL_Quit)]).


:- foreign(sdl_CreateWindow_C(+codes,
			      +integer,
			      +integer,
			      +integer,
			      +integer,
			      +integer,
			      -positive),
	   [fct_name(gp_SDL_CreateWindow)]).


:- foreign(sdl_DestroyWindow(+positive), [fct_name(gp_SDL_DestroyWindow)]).


:- foreign(sdl_CreateRenderer_C(+positive,
				 +integer,
				 +integer,
				 -positive),
	   [fct_name(gp_SDL_CreateRenderer)]).


:- foreign(sdl_DestroyRenderer(+positive), [fct_name(gp_SDL_DestroyRenderer)]).


:- foreign(sdl_CreateWindowAndRenderer_C(+integer,
					 +integer,
					 +integer,
					 -positive,
					 -positive),
	   [fct_name(gp_SDL_CreateWindowAndRenderer)]).


:- foreign(sdl_Delay(+positive), [fct_name(gp_SDL_Delay)]).

:- foreign(sdl_SetWindowTitle(+positive, +codes), [fct_name(gp_SDL_SetWindowTitle)]).