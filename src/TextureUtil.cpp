#include "TextureUtil.h"
#include <iostream>
#include <typeinfo>
#include <tinyxml2.h>


sf::Texture* TextureUtil::LoadTexture(const std::string& texturePath) { 

    static std::unordered_map<std::string, sf::Texture> textures;

    if (textures.find(texturePath) != textures.end()) {
        return &textures[texturePath];
    }
    sf::Texture texture;
    if(!texture.loadFromFile(texturePath))
        throw std::runtime_error("Failed to load texture : " + texturePath);

    textures.emplace(texturePath, texture);
    return &textures[texturePath];
}


std::unordered_map<std::string, sf::IntRect> LoadTextureAtlas(const std::string& xmlPath) {
    std::unordered_map<std::string, sf::IntRect> sprites;
    tinyxml2::XMLDocument doc;
    
    if (doc.LoadFile(xmlPath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << xmlPath << std::endl;
        return sprites;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("TextureAtlas");
    if (!root) {
        std::cerr << "Invalid XML format: No <TextureAtlas> found." << std::endl;
        return sprites;
    }

    for (tinyxml2::XMLElement* sprite = root->FirstChildElement("SubTexture"); sprite; sprite = sprite->NextSiblingElement("SubTexture")) {
        std::string name = sprite->Attribute("name");
        int x = sprite->IntAttribute("x");
        int y = sprite->IntAttribute("y");
        int width = sprite->IntAttribute("width");
        int height = sprite->IntAttribute("height");

        sprites[name] = sf::IntRect{{x, y}, {width, height}};
    }

    return sprites;
}