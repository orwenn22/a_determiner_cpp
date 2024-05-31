raylib = raylib/src

src_files = ./src/*.cpp \
            ./src/actions/*.cpp \
            ./src/engine/*.cpp \
            ./src/engine/metrics/*.cpp \
            ./src/engine/object/*.cpp \
            ./src/engine/state/*.cpp \
            ./src/engine/util/*.cpp \
            ./src/engine/widgets/*.cpp \
            ./src/engine/windows/*.cpp \
            ./src/menus/*.cpp \
            ./src/objects/actors/*.cpp \
            ./src/objects/collectibles/*.cpp \
            ./src/utils/*.cpp \
            ./src/widgets/*.cpp

# require EMSDK to be in the PATH
# TODO: make the changes requiered to remove ASYNCIFY
adeterminer.html: ${src_files}
	emcc ${src_files} ./raylib/src/libraylib.a \
		-o adeterminer.html \
		-I${raylib} -Isrc -s USE_GLFW=3 -s ASYNCIFY \
		--shell-file shell.html \
		-sGL_ENABLE_GET_PROC_ADDRESS \
		--preload-file ./res/menubg.png \
		--preload-file ./res/tiled_button.png \
		--preload-file ./res/wowmap.png
