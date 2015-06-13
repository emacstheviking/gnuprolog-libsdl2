:- initialization(prep).

prep :-
	sdl_init([video]).


check_sdl :-
	sdl_createwindow("fred", undefined, centered, 640, 480, [], W),
	sdl_delay(2000),
	sdl_destroywindow(W).


