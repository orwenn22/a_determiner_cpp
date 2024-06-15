#ifndef UI_FRAMEWORK_PATHS_H
#define UI_FRAMEWORK_PATHS_H

#include <string>

std::string GetCWD();
std::string RemoveFileFromPath(std::string file_path);
std::string GetFileFromPath(std::string file_path);
std::string GetExtensionFromPath(std::string file_path);

bool TryCreateDirectory(std::string path);

#endif //UI_FRAMEWORK_PATHS_H
