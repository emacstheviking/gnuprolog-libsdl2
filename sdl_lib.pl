
%% Start and stop SDL
:- foreign(sdl_init(+term),
	   [fct_name(gp_sdl_init),
	    return(boolean)]).

:- foreign(sdl_quit,
	   [fct_name(gp_sdl_quit),
	    return(boolean)]).

%% Window creation
%%
%% Returns true if the window created OK, the window handle will be
%% unified with the last variable, W.
%%
:- foreign(sdl_createwindow(+term,    %% title
			    +term,    %% x
			    +term,    %% y
			    +integer, %% w
			    +integer, %% h
			    +term,    %% [ atom-flags ]
			    +term),   %% WindowHandle
	   [
	    fct_name(gp_createwindow),
	    return(boolean)
	   ]).
