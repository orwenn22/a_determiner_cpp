#include "IDBFS.h"

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

void MountIDBFS() {
    // From https://stackoverflow.com/questions/54617194/how-to-save-files-from-c-to-browser-storage-with-emscripten
    //FIXME : not working (?)
    EM_ASM(
        // Make a directory other than '/'
        FS.mkdir('/idbfs');
        // Then mount with IDBFS type
        FS.mount(IDBFS, {}, '/idbfs');

        // Then sync
        FS.syncfs(true, function (err) {
            console.log("Failed to mount idbfs, things are most likely going to break");
            console.log(err);
        });
    );
}

#else

void MountIDBFS() {
    //Nothing to do if we are not using emscripten
}

#endif
