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

sdl_constants('Mix_Init',
	      [ mix_init_flac        - 0x00000001
	      , mix_init_mod         - 0x00000002
	      , mix_init_modplug     - 0x00000004
	      , mix_init_mp3         - 0x00000008
	      , mix_init_ogg         - 0x00000010
	      , mix_init_fluidsynth  - 0x00000020
	      ]).

sdl_constants('IMG_Init',
	      [ jpg  - 0x00000001
	      , png  - 0x00000002
	      , tif  - 0x00000004
	      , webp - 0x00000008
	      ]).
