%%====================================================================
%%
%%                            FFI DEFINITIONS
%%
%%====================================================================

:- foreign(sdl_Init_C(+positive), [fct_name(gp_SDL_Init)]).
:- foreign(sdl_Quit,              [fct_name(gp_SDL_Quit)]).


%% Window functions

:- foreign(sdl_CreateWindow_C(+codes, +integer, +integer, +integer, +integer, +integer, -positive),
	   [fct_name(gp_SDL_CreateWindow)]).

:- foreign(sdl_DestroyWindow(+positive), [fct_name(gp_SDL_DestroyWindow)]).

:- foreign(sdl_SetWindowTitle(+positive, +codes), [fct_name(gp_SDL_SetWindowTitle)]).


%% Render functions

:- foreign(sdl_CreateRenderer_C(+positive, +integer, +integer, -positive),
	   [fct_name(gp_SDL_CreateRenderer)]).

:- foreign(sdl_DestroyRenderer(+positive), [fct_name(gp_SDL_DestroyRenderer)]).

:- foreign(sdl_CreateWindowAndRenderer_C(+integer, +integer, +integer, -positive, -positive),
	   [fct_name(gp_SDL_CreateWindowAndRenderer)]).

:- foreign(sdl_SetRenderDrawColor(+positive, +integer, +integer, +integer, +integer),
	   [fct_name(gp_SDL_SetRenderDrawColor)]).

:- foreign(sdl_RenderClear(+positive), [fct_name(gp_SDL_RenderClear)]).
:- foreign(sdl_RenderPresent(+positive), [fct_name(gp_SDL_RenderPresent)]).

:- foreign(sdl_RenderDrawPoint(+positive, +integer, +integer),
	   [fct_name(gp_SDL_RenderDrawPoint)]).

:- foreign(sdl_RenderDrawLine(+positive, +integer, +integer, +integer, +integer),
	   [fct_name(gp_SDL_RenderDrawLine)]).

:- foreign(sdl_RenderDrawRect(+positive, +integer, +integer, +integer, +integer),
	  [fct_name(gp_SDL_RenderDrawRect)]).

:- foreign(sdl_RenderFillRect(+positive, +integer, +integer, +integer, +integer),
	   [fct_name(gp_SDL_RenderFillRect)]).


%% Surface and Texture functions

:- foreign(sdl_CreateTexture(+positive, +integer, +integer, +integer, +integer, -positive),
	   [fct_name(gp_SDL_CreateTexture_C)]).

:- foreign(sdl_CreateTextureFromSurface(+positive, +positive, -positive),
	   [fct_name(gp_SDL_CreateTextureFromSurface)]).

:- foreign(sdl_FreeSurface(+positive),
	   [fct_name(gp_SDL_FreeSurface)]).

:- foreign(sdl_RenderCopyDefaults(+positive, +positive),
	   [fct_name(gp_SDL_RenderCopyDefaults)]).

:- foreign(sdl_RenderCopy(+positive, +positive, +positive, +positive, +positive,
			  +positive, +positive, +positive, +positive, +positive),
	   [fct_name(gp_SDL_RenderCopyDefaults)]).


%% Event functions

:- foreign(sdl_PollEvent(-term), [fct_name(gp_SDL_PollEvent)]).


%% Text input functions

:- foreign(sdl_StartTextInput, [fct_name(gp_SDL_StartTextInput)]).
:- foreign(sdl_StopTextInput,  [fct_name(gp_SDL_StopTextInput)]).

:- foreign(sdl_SetTextInputRect(+integer, +integer, +integer, +integer),
	   [fct_name(gp_SDL_SetTextInputRect)]).


%% LoadBMP

:- foreign(sdl_LoadBMP(+codes, -positive),  [fct_name(gp_SDL_LoadBMP)]).


%% General functions

:- foreign(sdl_Delay(+positive),   [fct_name(gp_SDL_Delay)]).
:- foreign(sdl_GetTicks(-integer), [fct_name(gp_SDL_GetTicks)]).

:- foreign(sdl_ShowSimpleMessageBox_C(+integer, +codes, +codes),
	   [fct_name(gp_SDL_ShowSimpleMessageBox_C)]).

:- foreign(sdl_GetPlatform(-term), [fct_name(gp_SDL_GetPlatform)]).


%% Window and Display Functions

:- foreign(sdl_GetNumVideoDisplays, [fct_name(gp_SDL_GetNumVideoDisplays)]).

:- foreign(sdl_GetDisplayBounds(+positive, -positive, -positive, -positive, -positive),
	   [fct_name(gp_SDL_GetDisplayBounds)]).

:- foreign(sdl_SetWindowFullScreen_C(+positive, +positive),
	   [fct_name(gp_SDL_SetWindowFullScreen)]).

:- foreign(sdl_GetWindowFlags(+positive, -positive),
	   [fct_name(gp_SDL_GetWindowFlags)]).


%% Audio functions

:- foreign(sdl_GetNumAudioDevices(+positive, -positive),  [fct_name(gp_SDL_GetNumAudioDevices)]).
:- foreign(sdl_GetNumAudioDrivers(-positive),             [fct_name(gp_SDL_GetNumAudioDevices)]).


%% ADDITIONS to "SDL" for your convenience!

:- foreign(sdl_RenderDrawCircle(+positive, +positive, +positive, +positive),
	  [fct_name(gp_SDL_RenderDrawCircle)]).

:- foreign(sdl_RenderFillCircle(+positive, +positive, +positive, +positive),
	  [fct_name(gp_SDL_RenderFillCircle)]).
