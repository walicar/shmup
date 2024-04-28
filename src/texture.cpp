#include "texture.h"

Texture::Texture() :
        width(0), height(0),
        internal_format(GL_RGB),
        image_format(GL_RGB),
        wrap_s(GL_REPEAT), wrap_t(GL_REPEAT),
        filter_min(GL_LINEAR), filter_max(GL_LINEAR) {
    glGenTextures(1, &ID);
}

void Texture::generate(unsigned int width, unsigned int height, unsigned char *data) {
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, this->width, this->height, 0,
                 image_format, GL_UNSIGNED_BYTE, data);

    // setting wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0); // not sure if we need this?
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}