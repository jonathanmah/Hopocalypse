#include "fx/DamageNumber.h"

DamageNumber::DamageNumber(sf::Vector2f position, int damage, int size, sf::Color colour, sf::Font& font)
: position(position), 
damage(damage), 
colour(colour), 
fadeTime(.7f), 
speed(50.f),
text(font)
{
    //text.setString(std::to_string(damage));
    //text.setString("MAX AMMO");
    text.setFont(font);
    text.setCharacterSize(size);
    text.setPosition(position);
    text.setFillColor(colour);
    
}
void DamageNumber::DrawToRenderTexture(sf::RenderTexture& renderTexture) {
    renderTexture.draw(text);
}

void DamageNumber::Update(float deltaTime) {
    position.y -= speed * deltaTime;
    text.setPosition(position);
    
    sf::Color color = text.getFillColor();
    color.a = std::max(0.f, color.a - (255.f / fadeTime) * deltaTime);
    text.setFillColor(color);
}

bool DamageNumber::IsExpired() {
    return text.getFillColor().a <= 0;
}