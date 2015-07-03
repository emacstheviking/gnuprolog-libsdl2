:- initialization(prep).

prep :-
	sdl_Init([video]).


check_sdl :-
	sdl_CreateWindow("fred", 0, 0, 640, 480, [], W),
	sdl_Delay(2000),
	sdl_SetWindowTitle(W, "Another title"),
	sdl_Delay(2000),
	sdl_DestroyWindow(W).
