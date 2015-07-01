.PHONY: tags

.DEFAULT_GOAL := sdl

tags:
	rm -f TAGS
	find /usr/local/include/SDL2/ -name "*.h" | etags -
	find ~/Documents/RUNTIMES/gprolog-1.4.4/src -name "*.h" | etags -a -
	find ~/Documents/RUNTIMES/gprolog-1.4.4/src -name "*.c" | etags -a -

sdl:
	gplc -L -lsdl2 -o sdltest sdl.pl sdl_lib.c

clean:
	rm *~
	rm -fr html

doc:
	doxygen Doxyfile
