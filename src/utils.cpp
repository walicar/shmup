#include "utils.h"
namespace safety {
    void auto_throw(std::string context) {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::string message = context + " - OpenGL Error Codes: ";
            bool first = true;
            while (err != GL_NO_ERROR) {
                message += std::to_string(err);
                err = glGetError();
            }
            throw std::runtime_error(message);
        }
    }

    void entry_guard(std::string fn_name) {
        if (true) {
            std::string context = "Encountered OpenGL error from before '";
            context += fn_name + "' was called.";
            auto_throw(context);
        }
    }

    void exit_guard(std::string fn_name) {
        if (true) {
            std::string context = "Encountered OpenGL error during evaluation of '";
            context += fn_name + "'.";
            auto_throw(context);
        }
    }
}