%%--------------------------------------------------------------------
%%
%% Prolog wrappers for sdl.pl to call down to after validation etc.
%%
%% gp_sdl_init                  SDL_Init
%% gp_sdl_createwindow          SDL_CreateWindow      
%% gp_sdl_destroywindow         SDL_DestroyWindow
%% gp_sdl_delay                 SDL_Delay
%%
%%--------------------------------------------------------------------

:- foreign(gp_sdl_init(+positive),
	   [fct_name(gp_sdl_init), return(boolean)]).

:- foreign(gp_sdl_createwindow(+codes,
			       +integer, +integer,
			       +integer, +integer,
			       +integer, -positive),
	   [fct_name(gp_createwindow), return(boolean)]).

:- foreign(sdl_destroywindow(+positive),
	   [fct_name(gp_destroywindow), return(boolean)]).

:- foreign(sdl_delay(+positive),
	   [fct_name(gp_delay), return(boolean)]).

%%--------------------------------------------------------------------
%%
%% Pure C implementations
%%
%% There is no corresponding predicate in sdl.pl for the following
%% functions, they are pure C functions for that particular feature.
%%
%%--------------------------------------------------------------------

:- foreign(sdl_quit,
	   [fct_name(gp_sdl_quit), return(boolean)]).

