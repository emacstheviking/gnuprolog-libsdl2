.PHONY: tags

.DEFAULT_GOAL := sdl

tags:
	rm -f TAGS
	find ./SDL2/gp/include/SDL2 -name "*.h" | etags -
	find ~/Documents/RUNTIMES/gprolog-1.4.4/src -name "*.h" | etags -a -
	find ~/Documents/RUNTIMES/gprolog-1.4.4/src -name "*.c" | etags -a -

sdl:
	gplc \
	-C -I./SDL2/gp/include \
	-L -L./SDL2/gp/lib \
	-L -lSDL2 -L -lSDL2_ttf -L -lSDL2_image -o sdltest \
	sdl-ffi.pl sdl.pl sdl_lib.c

clean:
	rm *~
	rm -fr html

doc:
	doxygen Doxyfile
