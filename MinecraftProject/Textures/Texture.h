//
// Created by amit on 4/22/2022.
//
#ifndef MINECRAFTPROJECT_TEXTURE_H
#define MINECRAFTPROJECT_TEXTURE_H
#include <glad/gl.h>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

class Texture {
private:
    static unordered_map<string, Texture> texturesCache;
    GLuint textureID;
public:
    explicit Texture(GLuint textureID);
    static Texture& loadTexture(const string& fileName);
    [[nodiscard]] GLuint getTextureID() const;
    void use() const;
    bool operator == (const Texture& other) const;
};

template<>
struct std::hash<Texture> {
    size_t operator()(const Texture& texture) const {
        return std::hash<GLuint>()(texture.getTextureID());
    }
};


#endif //MINECRAFTPROJECT_TEXTURE_H
