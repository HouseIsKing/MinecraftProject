#pragma once
#include "IncludeSorter.h"
#include <memory>
#include <string>
#include <unordered_map>

struct TextureHasher;

class Texture
{
    static std::unordered_map<std::string, std::unique_ptr<Texture>> TexturesCache;
    GLuint64 Handle;
    GLuint TextureId;

public:
    Texture(GLuint64 handle, GLuint textureId);
    static Texture* LoadTexture(const std::string& path);
    [[nodiscard]] GLuint64 GetHandle() const;
    void Resident() const;
    void NonResident() const;
    bool operator==(const Texture& other) const;
    ~Texture();
    Texture(const Texture& other) = default;
    Texture(Texture&& other) = default;
    Texture& operator =(const Texture& other) = default;
    Texture& operator =(Texture&& other) = default;
};

struct TextureHasher
{
    size_t operator()(const Texture* texture) const noexcept
    {
        return std::hash<GLuint64>()(texture->GetHandle());
    }
};
