#include "util/TextureUtil.h"
#include <iostream>
#include <typeinfo>
#include "fx/Blood.h"

sf::Texture* TextureUtil::GetTexture(const std::string& texturePath) {
    static std::unordered_map<std::string, sf::Texture> textures; //why has to be func scoped and not class scoped? keeps failing
    if (textures.find(texturePath) != textures.end()) {
        return &textures[texturePath];
    }
    std::cout <<"failed to find  " << texturePath << std::endl;
    sf::Texture texture;
    if(!texture.loadFromFile(texturePath))
        throw std::runtime_error("Failed to load texture : " + texturePath);

    textures.emplace(texturePath, texture);
    return &textures[texturePath];
}

void TextureUtil::SetStaticMemberTextures() {
    Blood::texture = GetTexture("../assets/textures/fx/blood_atlas.png");
}