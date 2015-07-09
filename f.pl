check_sdl :-
	sdl_Init([video]),
	WndW=640,
	WndH=480,
	sdl_CreateWindow("fred", 0, 0, WndW, WndH, [], W),
	evloop,
	sdl_DestroyWindow(W),
	sdl_Quit.

drawtest :-
	sdl_Init([video]),
	WndW=640,
	WndH=480,
	sdl_CreateWindow("fred", 0, 0, WndW, WndH, [], W),
	sdl_CreateRenderer(W, -1, [], R),
	sdl_SetRenderDrawColor(R, 0, 0, 0, 255),
	sdl_RenderClear(R),
	evloop(drawstuff(R, WndW, WndH)),
	sdl_DestroyRenderer(R),
	sdl_DestroyWindow(W),
	sdl_Quit.

drawstuff(R, WndW, WndH) :-
	draw_a_circle(R, WndW, WndH),
	sdl_RenderPresent(R).


circletest :-
	sdl_Init([video]),
	WndW=640,
	WndH=480,
	sdl_CreateWindow("fred", 0, 0, WndW, WndH, [], W),
	sdl_CreateRenderer(W, -1, [], R),
	sdl_SetRenderDrawColor(R, 0, 0, 0, 255),
	sdl_RenderClear(R),
	sdl_SetRenderDrawColor(R, 255, 255, 255, 255),
	sdl_RenderDrawCircle(R, 320, 240, 100),
	sdl_RenderPresent(R),
	sdl_Delay(60000),
	sdl_DestroyRenderer(R),
	sdl_DestroyWindow(W),
	sdl_Quit.

draw_a_box(R, WndW, WndH) :-
	random(0, WndW, X),
	random(0, WndH, Y),
	random(50, 300, W),
	random(50, 200, H),
	random(0, 255, Red),
	random(0, 255, Grn),
	random(0, 255, Blu),
	sdl_SetRenderDrawColor(R, Red, Grn, Blu, 255),
	sdl_RenderFillRect(R, X, Y, W, H).

draw_a_circle(R, WndW, WndH) :-
	random(0, WndW, X),
	random(0, WndH, Y),
	random(50, 300, Radius),
	random(0, 255, Red),
	random(0, 255, Grn),
	random(0, 255, Blu),
	sdl_SetRenderDrawColor(R, Red, Grn, Blu, 255),
	sdl_RenderDrawCircle(R, X, Y, Radius).

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


%%--------------------------------------------------------------------
%%
%% Textures
%%
%%--------------------------------------------------------------------
textest :-
	sdl_Init([video]),
	sdl_CreateWindow("fred", 0, 0, 640, 480, [], W),
	sdl_CreateRenderer(W, -1, [accelerated, presentvsync], R),
	sdl_CreateTexture(T),


	sdl_DestroyTexture(T),
	sdl_DestroyRenderer(R),
	sdl_DestroyWindow(W),
	sdl_Quit.


%%--------------------------------------------------------------------
%%
%% Event Handling
%%
%%--------------------------------------------------------------------
evtest :-
	sdl_Init([video]),
	evloop,
	sdl_Quit.

evloop :-
	sdl_PollEvent(E),
	handle_event(E),
	%%sdl_Delay(25),
	!,
	evloop.
evloop. %% keep going on poll fail i.e. no event!

evloop(Callback) :-
	call(Callback),
	sdl_PollEvent(E),
	handle_event(E),
	sdl_Delay(1),
	!,
	evloop(Callback).

evloop(_).

%%--------------------------------------------------------------------
%%
%% Event handlers...
%%
%%--------------------------------------------------------------------
handle_event(quit(_)) :-
	format("QUIT selected~n", []), !, fail.

handle_event(keyup(_,_,released,_,_,27,_)) :-
	format("ESC pressed~n", []), !, fail.

handle_event(_).
