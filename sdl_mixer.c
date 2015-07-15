#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gprolog.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include "sdllib_common.h"


#define RETURN_MIX_FAIL(F)					\
  fprintf(stderr, "%s failed -> %s\n", #F, Mix_GetError());	\
  return PL_FALSE;


#define SHOW_MIX_FAIL(F) fprintf(stderr, "%s failed -> %s\n", #F, Mix_GetError());


//--------------------------------------------------------------------
//
//                    Mixer functions
//
//                      "SDL_mixer"
//
//--------------------------------------------------------------------
PlBool gp_Mix_Linked_Version(PlLong *major, PlLong *minor, PlLong *patch)
{
  SDL_version compile_version;
  const SDL_version *link_version = Mix_Linked_Version();
  SDL_MIXER_VERSION(&compile_version);  

  *major = (PlLong)compile_version.major;
  *minor = (PlLong)compile_version.minor;
  *patch = (PlLong)compile_version.patch;
  
  return PL_TRUE;
}


PlBool gp_Mix_Init(PlLong flags)
{
  int result = Mix_Init(flags);

  if (result) {
    RETURN_MIX_FAIL(Mix_Init);
  }
  return PL_TRUE;
}


PlBool gp_Mix_Quit()
{
  Mix_Quit();
  return PL_TRUE;
}


PlBool gp_Mix_OpenAudio(
    PlLong frequency, PlLong format,
    PlLong channels, PlLong chunks)
{
  if (0 == format) {
    format = MIX_DEFAULT_FORMAT;
  }
  
  if (Mix_OpenAudio((int)frequency, (int)format, (int)channels, (int)chunks)) {
    RETURN_MIX_FAIL(Mix_OpenAudio);
  }
  return PL_TRUE;
}


PlBool gp_Mix_CloseAudio()
{
  Mix_CloseAudio();
  return PL_TRUE;
}


//--------------------------------------------------------------------
//
// "4.5 Music Playing"
//
//--------------------------------------------------------------------
PlBool gp_Mix_LoadMUS(char *filename, PlLong *handle)
{
  Mix_Music *music = Mix_LoadMUS(filename);

  if (0 == music) {
    RETURN_MIX_FAIL(Mix_LoadMUS);
  }
  *handle = (PlLong)music;
  return PL_TRUE;
}


PlBool gp_Mix_FreeMusic(PlLong *handle)
{
  Mix_FreeMusic((Mix_Music*)handle);
  return PL_TRUE;
}


PlBool gp_Mix_PlayMusic(PlLong handle, PlLong loops)
{
  if (-1 == Mix_PlayMusic((Mix_Music*)handle, (int)loops)) {
    RETURN_MIX_FAIL(Mix_PlayMusic);
  }
  return PL_TRUE;
}


PlBool gp_Mix_FadeInMusic(PlLong handle, PlLong loops, PlLong ms)
{
  if (-1 == Mix_FadeInMusic((Mix_Music*)handle, (int)loops, (int)ms)) {
    RETURN_MIX_FAIL(Mix_FadeInMusic);
  }
  return PL_TRUE;
}


PlBool gp_Mix_FadeOutMusic(PlLong ms)
{
  if (-1 == Mix_FadeOutMusic((int)ms)) {
    RETURN_MIX_FAIL(Mix_FadeOutMusic);
  }
  return PL_TRUE;
}


PlBool gp_Mix_FadeInMusicPos(PlLong handle, PlLong loops, PlLong ms, double pos)
{
  if (-1 == Mix_FadeInMusicPos((Mix_Music*)handle, (int)loops, (int)ms, pos)) {
    RETURN_MIX_FAIL(Mix_FadeInMusicPos);
  }
  return PL_TRUE;
}


PlBool gp_Mix_VolumeMusic(PlLong volume, PlLong *oldVolume)
{
  *oldVolume = (PlLong)Mix_VolumeMusic((int)volume);
  return PL_TRUE;
}


PlBool gp_Mix_HaltMusic()
{
  Mix_HaltMusic();
  return PL_TRUE;
}


PlBool gp_Mix_PauseMusic()
{
  Mix_PauseMusic();
  return PL_TRUE;
}

PlBool gp_Mix_ResumeMusic()
{
  Mix_ResumeMusic();
  return PL_TRUE;
}

PlBool gp_Mix_RewindMusic()
{
  Mix_RewindMusic();
  return PL_TRUE;
}


PlBool gp_Mix_QuerySpec(PlLong *frequency, PlLong *format, PlLong *channels, PlLong *opened)
{
  int hz, chn;
  Uint16 fmt;

  int open = Mix_QuerySpec(&hz, &fmt, &chn);

  if (0 == open) {
    RETURN_MIX_FAIL(Mix_QuerySpec);
  }

  *frequency = (PlLong)hz;
  *format    = (PlLong)fmt;
  *channels  = (PlLong)chn;
  *opened    = (PlLong)open;
  return PL_TRUE;
}


//--------------------------------------------------------------------
//
// "4.2 Samples"
//
//--------------------------------------------------------------------
PlBool gp_Mix_GetNumChunkDecoders(PlLong *count)
{
  *count = (PlLong)Mix_GetNumChunkDecoders();
  return PL_TRUE;
}


PlBool gp_Mix_GetChunkDecoder(PlLong index, PlTerm *name)
{
  const char *decoderName = Mix_GetChunkDecoder((int)index);
  if (decoderName) {
    *name = Pl_Create_Atom(decoderName);
    return PL_TRUE;
  }
  return PL_FALSE;
}


PlBool gp_Mix_LoadWAV(char* filename, PlLong *sample)
{
  Mix_Chunk *chunk = Mix_LoadWAV(filename);

  if (chunk) {
    *sample = (PlLong)chunk;
    return PL_TRUE;
  }
  RETURN_MIX_FAIL(Mix_LoadWAV);
}


PlBool gp_Mix_FreeChunk(PlLong chunk)
{
  Mix_FreeChunk((Mix_Chunk*)chunk);
  return PL_TRUE;
}


PlBool gp_Mix_VolumeChunk(PlLong chunk, PlLong volume, PlLong *oldVolume)
{
  *oldVolume = (PlLong)Mix_VolumeChunk((Mix_Chunk*)chunk, (int)volume);
  return PL_TRUE;
}


//--------------------------------------------------------------------
//
// "4.3 Channels"
//
//--------------------------------------------------------------------
PlBool gp_Mix_AllocateChannels(PlLong numchans, PlLong *actual)
{
  *actual = (PlLong)Mix_AllocateChannels((int)numchans);
  return PL_TRUE;
}


PlBool gp_Mix_Volume(PlLong channel, PlLong volume, PlLong *current)
{
  *current = (PlLong)Mix_Volume((int)channel, (int)volume);
  return PL_TRUE;
}


PlBool gp_Mix_PlayChannel(PlLong channel, PlLong chunk, PlLong loops, PlLong *out)
{
  int output = Mix_PlayChannel((int)channel, (Mix_Chunk*)chunk, (int)loops);

  if (-1 == output) {
    RETURN_MIX_FAIL(Mix_PlayChannel);
  }

  *out = (PlLong)output;
  return PL_TRUE;
}


PlBool gp_Mix_PlayChannelTimed(
    PlLong channel, PlLong chunk,
    PlLong loops, PlLong ticks,
    PlLong *out)
{
  int output = Mix_PlayChannelTimed((int)channel, (Mix_Chunk*)chunk, (int)loops, (int)ticks);

  if (-1 == output) {
    RETURN_MIX_FAIL(Mix_PlayChannelTimed);
  }

  *out = (PlLong)output;
  return PL_TRUE;
}

