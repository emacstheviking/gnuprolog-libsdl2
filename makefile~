.PHONY: tags

.DEFAULT_GOAL := sdl_osx

tags:
	rm -f TAGS
	find ./SDL2/gp/include/SDL2 -name "*.h" | etags -
	find ~/Documents/RUNTIMES/gprolog-1.4.4/src -name "*.h" | etags -a -
	find ~/Documents/RUNTIMES/gprolog-1.4.4/src -name "*.c" | etags -a -

sdl_osx:
	gplc \
	-C -I/Library/Frameworks/SDL2.framework/Headers \
	-C -I/Library/Frameworks/SDL2_ttf.framework/Headers \
	-C -I/Library/Frameworks/SDL2_mixer.framework/Headers \
	-C -I/Library/Frameworks/SDL2_image.framework/Headers \
	-L -framework SDL2 -L -framework SDL2_ttf -L -framework SDL2_image -L -framework SDL2_mixer -o sdltest \
	sdl-ffi.pl sdl.pl sdl_lib.c sdl_thread.c sdl_ttf.c sdl_mixer.c

clean:
	rm *~
	rm -fr html
	rm sdltest

doc:
	doxygen Doxyfile
