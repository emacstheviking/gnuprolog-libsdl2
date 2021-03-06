mixertest :-
	sdl_Init([audio]),
	mix_Linked_Version(Maj, Min, Patch),
	format("SDL_Mixer: ~w.~w.~w~n", [Maj, Min, Patch]),
	sdl_Init([audio]),
	mix_OpenAudio(22050, [default_format], 2, 4096),
	mix_LoadMUS("./assets/test-music.mp3", Music),
	format("Fading in the music now over 5 seconds: ~w~n....", [Music]),
	mix_VolumeMusic(128, _),
	mix_FadeInMusic(Music, 1, 5000),
	sdl_Delay(5000),
	format("Reduce volume to 50%% for two seconds~n", []),
	mix_VolumeMusic(64, _),
	sdl_Delay(2000),
 	format("Reduce volume to 25%% for two seconds~n", []),
	mix_VolumeMusic(32, _),
	sdl_Delay(2000),
 	format("Reduce volume to 6.25%% for two seconds~n", []),
	mix_VolumeMusic(8, _),
	sdl_Delay(2000),
	format("Max volume to 100%%, play three seconds then pause~n", []),
	mix_VolumeMusic(128, _),
	sdl_Delay(3000),
	mix_PauseMusic,
	sdl_Delay(3000),
	format("Resume playback~n", []),
	mix_ResumeMusic.


check_sdl :-
	sdl_Init([video]),
	WndW=640,
	WndH=480,
	sdl_CreateWindow("in the middle of the screen", centered, centered, WndW, WndH, [], W),
	evloop,
	sdl_DestroyWindow(W),
	sdl_Quit.

drawtest :-
	sdl_Init([video]),
	WndW=640,
	WndH=480,
	sdl_CreateWindow("fred", 0, 0, WndW, WndH, [], W),
	sdl_CreateRenderer(W, -1, [presentvsync], R),
	sdl_SetRenderDrawColor(R, 0, 0, 0, 255),
	sdl_RenderClear(R),
	evloop(drawstuff(R, WndW, WndH)),
	sdl_DestroyRenderer(R),
	sdl_DestroyWindow(W),
	sdl_Quit.

drawstuff(R, WndW, WndH) :-
	draw_a_box(R, WndW, WndH),
	sdl_RenderPresent(R).


circletest :-
	sdl_Init([video]),
	WndW=640,
	WndH=480,
	sdl_CreateWindow("fred", 0, 0, WndW, WndH, [], W),
	sdl_CreateRenderer(W, -1, [], R),
	sdl_SetRenderDrawColor(R, 0, 0, 0, 255),
	sdl_RenderClear(R),
	sdl_SetRenderDrawColor(R, 255, 0, 0, 255),
	sdl_RenderDrawCircle(R, 320, 240, 200),
	sdl_SetRenderDrawColor(R, 255, 0, 255, 255),
	sdl_RenderDrawCircle(R, 320, 240, 150),
	sdl_SetRenderDrawColor(R, 0, 255, 255, 255),
	sdl_RenderFillCircle(R, 320, 240, 100),
	sdl_SetRenderDrawColor(R, 0, 0, 255, 255),
	sdl_RenderFillCircle(R, 320, 240, 50),
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



textest(F) :-
	format_to_codes(Filename, "/Users/seancharles/Pictures/~s", [F]),
	sdl_Init([video]),
	img_Init,
	sdl_CreateWindow("Texture Test", undefined, undefined, 0, 0, [], W),
	sdl_CreateRenderer(W, -1, [accelerated, presentvsync], R),

	img_Load(Filename, Img),
	sdl_CreateTextureFromSurface(R, Img, Tex),
	sdl_QueryTexture(Tex, _, _, TW, TH),
	sdl_FreeSurface(Img),
	sdl_SetWindowSize(W, TW, TH),

	sdl_RenderCopyDefaults(R, Tex),
	sdl_RenderPresent(R),

	evloop,

	sdl_DestroyTexture(Tex),
	sdl_DestroyRenderer(R),
	sdl_DestroyWindow(W),
	img_Quit,
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
	!,
	evloop.
evloop. %% keep going on poll fail i.e. no event!

evloop(Callback) :-
	call(Callback),
	sdl_PollEvent(E),
	handle_event(E),
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



tt :-
	sdl_CreateThread(doit(42), "test-thread", X),
	write(X).

doit(N) :-
	write("in the thread"),
	write(N).


fonttest :-
	sdl_Init([video]),
	ttf_Init,
	sdl_CreateWindow("Font Tests", centered, centered, 640, 480, [], W),
	sdl_CreateRenderer(W, -1, [accelerated, presentvsync], R),
	%% clear to black
	sdl_SetRenderDrawColor(R, 0, 0, 0, 255),
	sdl_RenderClear(R),
	ttf_OpenFont("/Library/Fonts/OsakaMono.ttf",80, F),
	ttf_OpenFont("/Library/Fonts/Impact.ttf",40, F2),
	ttf_SizeUTF8(F, "Osaka Mono", RW, RH),
	format("Font texture: ~w x ~w pixels~n", [RW, RH]),
	sdl_SetRenderDrawColor(R, 255, 128, 128, 255),	
	ttf_RenderUTF8_Solid(R, F, 100, 0, "Osaka Mono"),
	sdl_SetRenderDrawColor(R, 0, 128, 128, 255),	
	ttf_RenderUTF8_Solid(R, F2, 100, RH, "Some other font"),
	sdl_RenderPresent(R),
	evloop,
	ttf_Quit,
	sdl_Quit.
