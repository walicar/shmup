#include <glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
    public:
        unsigned int ID;
        Shader(const char *vert_src, const char *frag_src);
        void use();
};