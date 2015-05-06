
%% Start and stop SDL 
:- foreign(sdl_init(+term), [fct_name(gp_sdl_init), return(boolean)]).
:- foreign(sdl_quit, [fct_name(gp_sdl_quit), return(boolean)]).
