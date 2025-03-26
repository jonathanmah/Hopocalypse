#include "fx/DamageNumberManager.h"
#include "fx/DamageNumber.h"
#include <iostream>

DamageNumberManager::DamageNumberManager() {
    if (!regFont.openFromFile("../assets/fonts/Roboto-Regular.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    regFont.setSmooth(false);
    if (!critFont.openFromFile("../assets/fonts/Roboto-Bold.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
    critFont.setSmooth(false);
}


void DamageNumberManager::AddDamageNumber(sf::Vector2f position, int damage, NumberType type){
    int size;
    sf::Color colour;
    switch(type) {
        case NumberType::REGULAR:
            damageNumbers.push_back(DamageNumber(position, damage, 18, sf::Color::White, regFont));
            break;
        case NumberType::CRITICAL:
            damageNumbers.push_back(DamageNumber(position, damage, 26, sf::Color::Yellow, critFont));
            break;
        default:
            return;
    }
    
}

void DamageNumberManager::Update(float deltaTime) {
    for (auto it = damageNumbers.begin(); it != damageNumbers.end();) {
        it->Update(deltaTime);
        if (it->IsExpired()) {
            it = damageNumbers.erase(it);
        } else {
            ++it;
        }
    }
}

void DamageNumberManager::Draw(sf::RenderWindow& window) {
    if(!renderTexture.resize(window.getSize())){
        throw std::runtime_error("Failed to resize font render texture");
    }
    renderTexture.clear(sf::Color::Transparent);
    for (auto& number : damageNumbers) {
        number.DrawToRenderTexture(renderTexture);
    }
    renderTexture.display();
    sf::Sprite sprite(renderTexture.getTexture());
    window.draw(sprite);
}