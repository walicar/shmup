#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <glad.h>

namespace safety {
    void auto_throw(std::string context);
    void entry_guard(std::string fn_name);
    void exit_guard(std::string fn_name);
}

#endif