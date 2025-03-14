#include "PlayerAnim.h"
#include <iostream>

PlayerAnim::PlayerAnim(){
    
    texture = TextureUtil::LoadTexture("../assets/textures/bunny_white.png");
    auto sprites = TextureUtil::LoadTextureAtlas("../assets/bunny.xml");

};

void PlayerAnim::InitTexture() {
    if (!texture->loadFromFile("spritesheet.png")) {
        std::cerr << "Failed to load texture atlas." << std::endl;
        return;
    }
}

sf::IntRect PlayerAnim::GetAnim(const std::string& name) {
    if (animations.find(name) == animations.end()) {
        std::cerr << "Sprite not found: " << name << std::endl;
        return;
    }
    return animations[name];
}