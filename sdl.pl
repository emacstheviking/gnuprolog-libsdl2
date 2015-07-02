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
%% All the Prolog predicates shown here are not all those available,
%% some are written in pure C code, see sdl_lib.c for details.
%%
%% My approach, after a pure C initial attempt, was to leverage the
%% power of Prolog to perform all parameter vetting etc to drastically
%% reduce the amount of C code I had to write, it's slower and more
%% error prone and of course it makes sense to use prolog for the
%% heavy lifting.
%%
%%
%% The end result is a core C library (sdl_lib.c) that does just
%% enough to call the corresponding SDL library function and return
%% the result into Prolog space.
%%
%% Any bug reports, improvements etc can be offered via GitHub.
%%
%%--------------------------------------------------------------------




%%--------------------------------------------------------------------
%%
%% SDL_Init
%%
%%--------------------------------------------------------------------
sdl_init :-
	sdl_init([everything]).

sdl_init(Flags) :-
	sdl_make_flags(Flags, sdl_constants, Value),
	gp_sdl_init(Value).


%%--------------------------------------------------------------------
%%
%% SDL_CreateWindow
%%
%%--------------------------------------------------------------------
sdl_createwindow(Title, X, Y, W, H, Flags, Wnd) :-
	var(Wnd),
	list(Title),
	atomic(X), \+ float(X),
	atomic(Y), \+ float(Y),
	integer(W),
	integer(H),
	list(Flags),
	sdl_make_flags(Flags, sdl_constants, Value),
	format("gp_sdl_createwindow(~w, ~w, ~w, ~w, ~w, ~w, Wnd).~n",
	       [Title, X, Y, W, H, Value]),
	gp_sdl_createwindow(Title, X, Y, W, H, Value, Wnd).






%%--------------------------------------------------------------------
%%
%% Internal support code for SDL module
%%
%%--------------------------------------------------------------------


%%--------------------------------------------------------------------
%% sdl_make_flags/3 -- helper
%%--------------------------------------------------------------------
sdl_make_flags([], _, 0).

sdl_make_flags(Flags, FlagProvider, Out) :-
	list(Flags),
	atom(FlagProvider),
	ReadFlags =.. [FlagProvider, Lookup],
	call(ReadFlags),
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

sdl_constants([
	       %%
	       %% SDL_Init
	       %%
	       timer          - 0x000001,
	       audio          - 0x000010,
	       video          - 0x000020,
	       joystick       - 0x000200,
	       haptic         - 0x001000,
	       gamecontroller - 0x002000,
	       events         - 0x004000,
	       everything     - 0x007231,
	       noparachute    - 0x100000,
	       %%
	       %% SDL_CreateWindow
	       %%
	       undefined      - 0x1fff0000,
	       centered       - 0x2fff0000
	      ]).
