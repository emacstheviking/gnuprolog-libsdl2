:- foreign(sdl_init(+term),
	   [fct_name(gp_sdl_init),
	    return(boolean)]).

:- foreign(sdl_quit,
	   [fct_name(gp_sdl_quit),
	    return(boolean)]).

:- foreign(sdl_createwindow(+codes,+term,+term,+integer,+integer,+term,-positive),
	   [fct_name(gp_createwindow),
	    return(boolean)]).

:- foreign(sdl_destroywindow(+positive),
	   [fct_name(gp_destroywindow),
	    return(boolean)]).

:- foreign(sdl_delay(+positive),
	   [fct_name(gp_delay),
	    return(boolean)]).
