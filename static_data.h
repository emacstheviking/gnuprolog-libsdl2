//--------------------------------------------------------------------
//
// SDL_Init() modes. The string table MUST be zero terminated and then
// the bit table must have a corresponding value to return.
//
//--------------------------------------------------------------------

char*  gstr_InitModes[] = {
  "timer",
  "audio",
  "video",
  "joystick",
  "haptic",
  "gamecontroller",
  "events",
  "everything",
  "noparachute",
  0
};

Uint32 gbit_InitModes[] = {
  SDL_INIT_TIMER,
  SDL_INIT_AUDIO,
  SDL_INIT_VIDEO,
  SDL_INIT_JOYSTICK,
  SDL_INIT_HAPTIC,
  SDL_INIT_GAMECONTROLLER,
  SDL_INIT_EVENTS,
  SDL_INIT_EVERYTHING,
  SDL_INIT_NOPARACHUTE
};


//--------------------------------------------------------------------
//
// SDL_CreateWindow() modes and positions.
//
//--------------------------------------------------------------------

char*  gstr_CwModes[] = {
  "fullscreen",
  "fullscreen_desktop",
  "opengl",
  "hidden",
  "borderless",
  "resizable",
  "minimized",
  "maximized",
  "input_grabbed",
  "allow_highdp",
  0
};

Uint32 gbit_CwModes[] = {
  SDL_WINDOW_FULLSCREEN,
  SDL_WINDOW_FULLSCREEN_DESKTOP,
  SDL_WINDOW_OPENGL,
  SDL_WINDOW_HIDDEN,
  SDL_WINDOW_BORDERLESS,
  SDL_WINDOW_RESIZABLE,
  SDL_WINDOW_MINIMIZED,
  SDL_WINDOW_MAXIMIZED,
  SDL_WINDOW_INPUT_GRABBED,
  SDL_WINDOW_ALLOW_HIGHDPI
};

char*  gstr_CwPos[] = {"undefined", "centered", 0};
Uint32 gbit_CwPos[] = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED};
