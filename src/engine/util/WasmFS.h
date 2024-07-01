#ifndef UI_FRAMEWORK_WASMFS_H
#define UI_FRAMEWORK_WASMFS_H

//This is only used by emscripten in order to achive permanent storage
void MountIDBFS();
void SyncIDBFS();

void DownloadFromFS(const char *path);

#endif //UI_FRAMEWORK_WASMFS_H
