#ifndef UI_FRAMEWORK_TRACE_H
#define UI_FRAMEWORK_TRACE_H

#include <cstdio>

// The __FILE__ macro display the entire path to the source file, which can be problematic for privacy.
// Therefore, it is better to check if __FILE_NAME__ (which does not display the full path, but is not
// a part of the standard) and use it if it is available. Else, there is a fallback trace function for
// compiler that does not support it.

#ifdef __FILE_NAME__
#define TRACE(...) printf("\033[3;90m%s %s L%d\033[0m ", __func__, __FILE_NAME__, __LINE__); printf(__VA_ARGS__)
#else
#define TRACE(...) printf("\033[3;90m%s L%d\033[0m ", __func__, __LINE__); printf(__VA_ARGS__)
#endif

#endif //UI_FRAMEWORK_TRACE_H
