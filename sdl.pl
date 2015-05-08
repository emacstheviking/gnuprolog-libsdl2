sdl_init(Flags) :-
	is_list(Flags),
	sdl_make_flags(Flags, sdl_init_flags, Value),
	gp_sdl_init(Value).

sdl_quit :- gp_sdl_quit.


%% for each entry in Flags
%%  get corresponding video bit mask
%%  bit wise OR to the total
%%  repeat until Flags is []


sdl_make_flags([], _, 0).

sdl_make_flags(Flags, FlagProvider, Out) :-
	ReadFlags =.. [FlagProvider, Lookup],
	call(ReadFlags),
	sdl_scan_flags(Flags, Lookup, 0, Out).


sdl_scan_flags([], _, Acc, Acc).
sdl_scan_flags([H|T], Lookup, Acc, Out) :-
	sdl_scan_map_lookup(H, Lookup),
	Acc2 is Acc \/ Bit,
	!, %% best position for the cut? can I lose it?
	sdl_scan_map_acc(T, Acc2, Out).

sdl_scan_flags([H|T], Lookup, Acc, Out) :-


sdl_init_flags([timer-0x000001, audio-0x000010, video-0x000020,
		joystick-0x000200, haptic-0x001000, gamecontroller-0x002000,
		events-0x004000, everything-0x007231, noparachute-0x100000]).
