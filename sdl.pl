%%--------------------------------------------------------------------
%% GNU Prolog SDL2 Module
%%
%% @author Sean James Charles
%%
%% This file contains the Prolog code that hopefulyl useable binding /
%% interface with libsdl2 for your platform. It was developed on OSX
%% and also tried on Ubuntu 14.04 LTS but has never been tried on
%% Windows. YMMV.
%%
%% My approach, after an initial attempt in pure C, has been to
%% leverage the power of Prolog to perform all parameter vetting etc
%% to drastically reduce the amount of C code I had to write, it's
%% slower to write and more error prone and of course it makes sense
%% to use Prolog for the heavy lifting.
%%
%% This module / binding also incorporates SDL utilities:
%%
%%      - SDL_ttf
%%      - SDL_image
%%      - SDL_mixer
%%
%% You now have not only the power of GNU Prolog at your disposal but
%% one of the most portable and easy-to-use graphics libraries ever
%% written IMHO.
%%
%%          START WRITING AWESOME APPLICATIONS NOW!   ;)
%%
%% Any bug reports, improvements etc can be offered via GitHub.
%%--------------------------------------------------------------------
sdl_Init :-
	sdl_Init([everything]).

sdl_Init(Flags) :-
	sdl_make_flags(Flags, 'SDL_Init', Value),
	sdl_Init_C(Value).




mix_OpenAudio :-
	mix_OpenAudio( 22050,		 % 22Khz sample rate
		       [default_format], % Use "default" audio format
		       2,		 % Number of channels
		       4096		 % Buffer size
		     ).

mix_OpenAudio(Freq, Flags, Channels, BufSize) :-
	sdl_make_flags(Flags, 'Mix_OpenAudio', Value),
	mix_OpenAudio_C(Freq, Value, Channels, BufSize).




sdl_CreateWindow(Title, X, Y, Width, Height, Flags, Wnd) :-
	var(Wnd),
	list(Title),
	atomic(X),
	\+ float(X),
	atomic(Y),
	\+ float(Y),
	sdl_make_flags([X], 'SDL_CreateWindow', XPos),
	sdl_make_flags([Y], 'SDL_CreateWindow', YPos),
	list(Flags),
	sdl_make_flags(Flags, 'SDL_CreateWindow', Value),
	sdl_CreateWindow_C(Title, XPos, YPos, Width, Height, Value, Wnd).




sdl_SetWindowFullScreen(Wnd, Flags) :-
	sdl_make_flags('SDL_SetWindowFullScreen', Flags, Value),
	sdl_SetWindowFullScreen_C(Wnd, Value).




sdl_CreateRenderer(Wnd, Index, Flags, Renderer) :-
	var(Renderer),
	nonvar(Wnd),
	integer(Index),
	list(Flags),
	sdl_make_flags(Flags, 'SDL_CreateWindow', Value),
	format("SDL.PL: gp_SDL_CreateRenderer_C(~w, ~w, ~w, Rndr).~n",
	       [Wnd, Index, Value]),
	sdl_CreateRenderer_C(Wnd, Index, Value, Renderer).




sdl_CreateWindowAndRenderer(Width, Height, Flags, Wnd, Rndr) :-
	var(Wnd),
	var(Rndr),
	list(Flags), %% no vars in list! ? check!!
	sdl_make_flags(Flags, 'SDL_CreateWindow', Value),
	format("SDL.PL: gp_SDL_CreateWindowAndRenderer_C(~w, ~w, ~w, Wnd, Rndr).~n",
	       [Width, Height, Value]),
	sdl_CreateWindowAndRenderer_C(Width, Height, Value, Wnd, Rndr).




sdl_RenderCopyEx(Renderer, Texture,
		 srcX, srcY, srcW, srcH,
		 dstX, dstY, dstW, dstH,
		 Angle,
		 CenterX, CenterY,
		 Flip) :-
	nonvar(Renderer), nonvar(Texture),
	integer(srcX), integer(srcY), integer(srcW), integer(srcH),
	integer(dstX), integer(dstY), integer(dstW), integer(dstH),
	number(Angle),
	list(Flip),
	sdl_make_flags(Flip, 'Sdl_RenderCopyEx', Value),
	sdl_RenderCopyEx_C(Renderer, Texture,
			   srcX, srcY, srcW, srcH,
			   dstX, dstY, dstW, dstH,
			   Angle,
			   CenterX, CenterY,
			   Value).




sdl_ShowSimpleMessageBox(Title, Message) :-
	sdl_ShowSimpleMessageBox(0, Title, Message, information).

sdl_ShowSimpleMessageBox(Wnd, Title, Message) :-
	sdl_ShowSimpleMessageBox(Wnd, Title, Message, information).

sdl_ShowSimpleMessageBox(Wnd, Title, Message, Mode) :-
	nonvar(Wnd),
	atom(Mode),
	list(Title),
	list(Message),
	sdl_make_flags([Mode], 'SDL_ShowSimpleMessageBox', Value),
	sdl_ShowSimpleMessageBox_C(Wnd, Title, Message, Value).




%%--------------------------------------------------------------------
%% sdl_make_flags/3 -- helper
%%--------------------------------------------------------------------
sdl_make_flags([], _, 0).

sdl_make_flags(Flags, FlagSet, Out) :-
	list(Flags),
	atom(FlagSet),
	sdl_constants(FlagSet, Lookup),
	sdl_scan_flags(Flags, Lookup, 0, Out).

sdl_make_flags(_,Where,_) :-
	Error =.. [Where, 'Check flags list and provider atom'],
	throw(Error).


%%--------------------------------------------------------------------
%% sdl_scan_flags/4 -- accumulator builder
%%--------------------------------------------------------------------
sdl_scan_flags([], _, Acc, Acc).

sdl_scan_flags([Flag|T], Lookup, Acc, Out) :-
	member(Flag-Bit, Lookup),
	Acc2 is Acc \/ Bit,
	sdl_scan_flags(T, Lookup, Acc2, Out).

sdl_scan_flags([_|T], Lookup, Acc, Out) :-
	sdl_scan_flags(T, Lookup, Acc, Out).


%%--------------------------------------------------------------------
%%
%% Bit-based flag lookup tables.
%%
%%--------------------------------------------------------------------
sdl_constants('SDL_Init',
	      [ timer           - 0x000001
	      , audio           - 0x000010
	      , video           - 0x000020
	      , joystick        - 0x000200
	      , haptic          - 0x001000
	      , gamecontroller  - 0x002000
	      , events          - 0x004000
	      , everything      - 0x007231
	      , noparachute     - 0x100000
	      ]).

sdl_constants('SDL_CreateWindow',
	      [ undefined       - 0x1fff0000
	      , centered        - 0x2fff0000
	      ]).

sdl_constants('SDL_ShowSimpleMessageBox',
	      [ error           - 0x00000010
	      , warning         - 0x00000020
	      , information     - 0x00000040
	      ]).

sdl_constants('SDL_CreateRenderer',
	      [ software        - 0x00000001
	      , accelerated     - 0x00000002
	      , presentvsync    - 0x00000004
	      , targettexture   - 0x00000008
	      ]).

sdl_constants('SDL_RenderCopyEx',
	      [ none            - 0x00000000
	      , flip_horizontal - 0x00000001
	      , flip_vertical   - 0x00000002
	      ]).

sdl_constants('SDL_CreateTexture',
	      [ software        - 0x00000001
	      , accelerated     - 0x00000002
	      , presentvsync    - 0x00000004
	      , targettexture   - 0x00000008
	      ]).

sdl_constants('SDL_SetWindowFullScreen',
	      [ fullscreen         - 0x00000001,
		fullscreen_desktop - 0x000010001
	      ]).

sdl_constants('SDL_GetWindowFlags',
	      [ fullscreen         - 0x00000001
	      , fullscreen_desktop - 0x00010001
	      , opengl             - 0x00000002
	      , shown              - 0x00000004
	      , hidden             - 0x00000008
	      , borderless         - 0x00000010
	      , resizable          - 0x00000020
	      , minimized          - 0x00000040
	      , maximized          - 0x00000080
	      , input_grabbed      - 0x00000100
	      , input_focus        - 0x00000200
	      , mouse_focus        - 0x00000400
	      , foreign            - 0x00000800
	      , allow_highdpi      - 0x00002000
	      ]).

sdl_constants('Mix_OpenAudio',
	      [ audio_u8           - 0x0008
	      , audio_s8           - 0x8008
	      , audio_u16lsb       - 0x0010
	      , audio_s16lsb       - 0x8010
	      , audio_u16msb       - 0x1010
	      , audio_s16msb       - 0x9010
	      , audio_u16          - 0x0010
	      , audio_s16          - 0x8010
	      , default_format     - 0
	      ]).
