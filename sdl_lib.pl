:- foreign(gp_sdl_init(+positive), [fct_name(gp_sdl_init), return(boolean)]).
:- foreign(gp_sdl_quit, [fct_name(gp_sdl_quit), return(boolean)]).

:- foreign(gp_sdl_createwindow(+codes, +integer, +integer, +integer, +integer, +term, -positive),
	   [fct_name(gp_createwindow),
	    return(boolean)]).

:- foreign(sdl_destroywindow(+positive), [fct_name(gp_destroywindow), return(boolean)]).

:- foreign(sdl_delay(+positive), [fct_name(gp_delay), return(boolean)]).
