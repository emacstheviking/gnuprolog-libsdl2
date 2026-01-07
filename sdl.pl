%% : vim : set ft=prolog
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

:- include(sdl_constants).


sdl_Init :-
	sdl_Init([everything]).

sdl_Init(Flags) :-
	sdl_make_flags(Flags, 'SDL_Init', Value),
	sdl_Init_C(Value).




mix_Init(Flags) :-
	sdl_make_flags(Flags, 'Mix_Init', Value),
	mix_Init_C(Value).




img_Init :-
	img_Init([jpg, png, tif, webp]).

img_Init(Flags) :-
	sdl_make_flags(Flags, 'IMG_Init', Value),
	img_Init_C(Value, Did),
	format("DID FLAGS: ~w~n", [Did]).




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
	sdl_make_flags(Flags, 'SDL_GetWindowFlags', Value),
	format("Window flags: ~w~n", [Value]),
	sdl_CreateWindow_C(Title, XPos, YPos, Width, Height, Value, Wnd).


sdl_SetWindowFullScreen(Wnd, Flag) :-
	nonvar(Wnd),
	%BUG? could this be switched up by accident?
	sdl_make_flags([Flag], 'SDL_SetWindowFullScreen', Value),
	sdl_SetWindowFullScreen_C(Wnd, Value).

sdl_SetRenderDrawBlendMode(Renderer, Flag):-
	nonvar(Renderer),
	sdl_make_flags([Flag], 'SDL_BlendMode', Value),
	sdl_SetRenderDrawBlendMode_C(Renderer, Value).

sdl_CreateRenderer(Wnd, Index, Flags, Renderer) :-
	var(Renderer),
	nonvar(Wnd),
	integer(Index),
	list(Flags),
	sdl_make_flags(Flags, 'SDL_CreateRenderer', Value),
	format("SDL.PL: gp_SDL_CreateRenderer_C(~w, ~w, ~w, Rndr).~n",
	       [Wnd, Index, Value]),
	sdl_CreateRenderer_C(Wnd, Index, Value, Renderer).




sdl_CreateWindowAndRenderer(Width, Height, Flags, Wnd, Rndr) :-
	var(Wnd),
	var(Rndr),
	list(Flags), %% no vars in list! ? check!!
	sdl_make_flags(Flags, 'SDL_GetWindowFlags', Value),
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


sdl_FlushEvent(Type):-
	atom(Type),
	sdl_make_flags([Type], 'SDL_EventType', Value),
	write('here 2'), nl,
	sdl_FlushEvent_C(Value).

sdl_FlushEvents(Mintype, Maxtype):-
	atom(Mintype), atom(Maxtype),
	sdl_make_flags([Mintype], 'SDL_EventType', Minval),
	sdl_make_flags([Maxtype], 'SDL_EventType', Maxval),
	write('here'),nl,
	sdl_FlushEvents_C(Minval, Maxval).


sdl_Log(Format) :-
	sdl_Log(Format, []).

sdl_Log(Format, Args) :-
	list(Format),
	list(Args),
	format_to_codes(Str, Format, Args),
	sdl_Log_C(Str).



sdl_LogMessage(Category, Priority, Format) :-
	sdl_LogMessage(Category, Priority, Format, []).

sdl_LogMessage(Category, Priority, Format, Args) :-
	list(Format),
	list(Args),
	atom(Category),
	atom(Priority),
	sdl_make_flags([Category], 'Log_Category', CatValue),
	sdl_make_flags([Priority], 'Log_Category', PriValue),
	format_to_codes(Str, Format, Args),
	sdl_LogMessage_C(CatValue, PriValue, Str).


%%--------------------------------------------------------------------
%% sdl_make_flags/3 -- helper to compose bit flags into a single value
%%--------------------------------------------------------------------
sdl_make_flags([], _, 0).

sdl_make_flags(Flags, FlagSet, Out) :-
	list(Flags),
	atom(FlagSet),
	sdl_constants(FlagSet, Lookup),
	sdl_scan_flags(Flags, Lookup, 0, Out).

sdl_make_flags(Flags, Where, _) :-
	Error =.. [Where, 'Check flags list and provider atom', Flags],
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
