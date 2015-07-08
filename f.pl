check_sdl :-
	sdl_Init([video]),
	sdl_CreateWindow("fred", 0, 0, 640, 480, [], W),
	evloop,
	sdl_Delay(2000),
	sdl_DestroyWindow(W),
	sdl_Quit.

drawtest :-
	sdl_Init([video]),
	sdl_CreateWindow("fred", 0, 0, 640, 480, [], W),
	sdl_CreateRenderer(W, -1, [], R),

	sdl_SetRenderDrawColor(R, 255, 255, 255, 255),
	sdl_RenderClear(R),
	%% a red dot
	sdl_SetRenderDrawColor(R, 255, 0, 0, 255),
	sdl_RenderDrawPoint(R, 10, 10),
	%% a blue unfilled rectangle
	sdl_SetRenderDrawColor(R, 0, 0, 255,255),
	sdl_RenderDrawRect(R, 20, 10, 200, 50),
	%% a green filled rectangle
	sdl_SetRenderDrawColor(R, 0, 128, 0,255),
	sdl_RenderFillRect(R, 230, 10, 200, 50),
	%%
	%% some random dots
	%%
	randomize,
	draw_a_dot(R, 100),
	
	sdl_RenderPresent(R),
	sdl_Delay(5000),	    
	sdl_DestroyRenderer(R),
	sdl_DestroyWindow(W),
	sdl_Quit.

draw_a_dot(_, 0) :-
	!.

draw_a_dot(R, N) :-
	N2 is N-1,
	random(0, 640, X),
	random(0, 480, Y),
	random(0, 255, Red),
	random(0, 255, Grn),
	random(0, 255, Blu),
	sdl_SetRenderDrawColor(R, Red, Grn, Blu, 255),
	sdl_RenderDrawPoint(R, X, Y),
	draw_a_dot(R, N2).


textest :-
	sdl_Init([video]),
	sdl_CreateWindow("fred", 0, 0, 640, 480, [], W),
	sdl_CreateRenderer(W, -1, [accelerated, presentvsync], R),
	sdl_CreateTexture(T),


	sdl_DestroyTexture(T),
	sdl_DestroyRenderer(R),
	sdl_DestroyWindow(W),
	sdl_Quit.

evtest :-
	sdl_Init([video]),
	evloop,
	sdl_Quit.

evloop :-
	sdl_PollEvent(E),
	handle_event(E),
	sdl_Delay(25),
	!,
	evloop.

evloop.


handle_event(keyup(_,_,released,_,_,27,_)) :-
	format("ESC pressed~n", []),
	!,
	fail.

handle_event(_).
