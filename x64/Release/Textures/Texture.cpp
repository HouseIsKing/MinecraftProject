//
// Created by amit on 4/22/2022.
//

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

using std::runtime_error;
using std::piecewise_construct;
using std::forward_as_tuple;

Texture* Texture::loadTexture(const string& path) {
    if(texturesCache.contains(path)) {
        return texturesCache.at(path).get();
    }
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    const unsigned char* pixels = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
    if (pixels == nullptr) {
        throw runtime_error("Failed to load texture");
    }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free((void *) pixels);
    texturesCache.emplace(piecewise_construct, forward_as_tuple(path), forward_as_tuple(new Texture(textureID)));
    return texturesCache.at(path).get();
}

Texture::Texture(GLuint textureID) : textureID(textureID) {
}

unordered_map<string, unique_ptr<Texture>> Texture::texturesCache{};

GLuint Texture::getTextureID() const {
    return textureID;
}

void Texture::use() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

bool Texture::operator==(const Texture &other) const {
    return textureID == other.textureID;
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureID);
}
