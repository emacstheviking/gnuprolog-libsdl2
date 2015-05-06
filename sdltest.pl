:- initialization(check_sdl).

check_sdl :-
	sdl_init([video]),
	sdl_window("fred", 0, 0, 640, 480),
	sdl_delay(2000),
	sdl_quit.


