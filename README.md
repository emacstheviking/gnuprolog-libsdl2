
# GNU Prolog / SDL2

Love Prolog, love graphics? Well this is for you!

I am really starting to enjoy Prolog a lot, especially GNU Prolog as
it is a fast, clean and very very easy to get to grips with
system. And quite stable too. It has one of the best FFI systems I've
ever seen and not only that, it can produce standalone native
executables with "gplc" as well. How much more could you want?

# OS X  Installation

I initially used "mac ports' version os the packages, that had
issues. Then I downloaded the source code from SDL site, that had
issues. In the end I remembered that you can use OS X Framework
packages from the command line from way back, so I downloaded the
various SDL and project .DMG files to my iMac, installed them into the
/Library/Frameworks folder as instructed and then after some trila and
errir I ended up with this command, which is now safely tucked away in
the Makefile:

	gplc \
	-C -I/Library/Frameworks/SDL2.framework/Headers \
	-C -I/Library/Frameworks/SDL2_ttf.framework/Headers \
	-C -I/Library/Frameworks/SDL2_mixer.framework/Headers \
	-C -I/Library/Frameworks/SDL2_image.framework/Headers \
	-L -framework SDL2 -L -framework SDL2_ttf -L -framework \
	SDL2_image -L -framework SDL2_mixer -o sdltest \
	sdl-ffi.pl sdl.pl sdl_lib.c sdl_thread.c sdl_ttf.c sdl_mixer.c


Basically it tells gplc program to pass through to the linker the
various frameworks needed to make it all build and link. For me on an
iMac running 10.10.4 on a Late 2012 model it all works just fine.


## Jack Server

You will need to download and install Jack for the audio to work it
seems. No, I don't know why it can't use Coreaudio...if I have done
something wrong then let me know as I'd love to have an OSX clean
environment.

I had to fiddle to make it work, and despite the fact that it does
work, it still says it can't connect to a server socket, no such file
or directory, blah blah.

Whatever.

I changed ssome "Setup" optoions and it seemed to work and has done
ever since. Perhaps I was just lucky. The settings I had to tweak were
accessed from the "Setup" button on the right hand side of the
"qjackclt" application UI, then on the main Settings tab:

  - Realtime is checked.
  - Verbose messages is checked
  - Frames/Period is 1024
  - Sample Rate 22050
  - Port Maximum is 256
  - Timeout (msec) is 500
  - Interface is (default)
  - Audio is Duplex
