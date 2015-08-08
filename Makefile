.PHONY: tags

.DEFAULT_GOAL := sdl_osx

tags:
	rm -f TAGS
	find /Library/Frameworks/SDL2.framework -name "*.h" | etags -
	find /Library/Frameworks/SDL2_ttf.framework  -name "*.h" | etags -
	find /Library/Frameworks/SDL2_mixer.framework  -name "*.h" | etags -
	find /Library/Frameworks/SDL2_image.framework  -name "*.h" | etags -

sdl_ubuntu:
	gplc \
	-C -I/usr/include/SDL2 \
	-L -L/usr/lib/x86_64-linux-gnu \
	-o sdltest \
	sdl-ffi.pl sdl.pl sdl_lib.c sdl_thread.c sdl_ttf.c sdl_mixer.c sdl_image.c \
	-L -lSDL2 \
	-L -lSDL2_ttf \
	-L -lSDL2_image \
	-L -lSDL2_mixer

sdl_osx:
	gplc \
	-C -I/Library/Frameworks/SDL2.framework/Headers \
	-C -I/Library/Frameworks/SDL2_ttf.framework/Headers \
	-C -I/Library/Frameworks/SDL2_mixer.framework/Headers \
	-C -I/Library/Frameworks/SDL2_image.framework/Headers \
	-L -framework SDL2 -L -framework SDL2_ttf -L -framework SDL2_image -L -framework SDL2_mixer -o sdltest \
	sdl-ffi.pl sdl.pl sdl_lib.c sdl_thread.c sdl_ttf.c sdl_mixer.c sdl_image.c

clean:
	rm *~
	rm -fr html
	rm sdltest

doc:
	doxygen Doxyfile

global:
	make sdl_osx
	sudo cp ./sdltest /opt/local/bin/gprolog

