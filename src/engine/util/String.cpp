#include "String.h"

#include <cstring>


void RemoveEndOfLine(char *str) {
    size_t line_len = strlen(str);
    if(line_len > 0 && str[line_len-1] == '\n') {
        str[line_len - 1] = 0;
    }
}

std::vector<std::string> SplitTokens(char *str) {
    std::vector<std::string> r;

    size_t len = strlen(str);
    std::string current_token = "";
    for(size_t i = 0; i < len; ++i) {
        if(str[i] == ' ' && !current_token.empty()) {
            r.push_back(current_token);
            current_token = "";
        }
        else {
            current_token.push_back(str[i]);
        }
    }

    if(!current_token.empty()) {
        r.push_back(current_token);
    }

    return r;
}
