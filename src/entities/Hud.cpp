#include "entities/Hud.h"
#include "util/HitboxDebugger.h"
#include <iostream>
#include <cmath>

static constexpr int HP_WIDTH_PIXELS = 85;
static constexpr int HP_HEIGHT_PIXELS = 6;
static constexpr float INTERPOLATION_FACTOR = .1f;
static constexpr float HP_Y_OFFSET = 10.f;
static constexpr int SEGMENT_LINE_THICKNESS = 1;
static constexpr int SEGMENT_SIZE = 100;

Hud::Hud(int maxHp, sf::Color healthColour, sf::Color damageColour) : maxHp(maxHp), currentHpWidth(HP_WIDTH_PIXELS), targetHpWidth(HP_WIDTH_PIXELS), damageHpWidth(HP_WIDTH_PIXELS){
    hpBar.setFillColor(healthColour);
    hpBar.setOrigin({HP_WIDTH_PIXELS/2, HP_HEIGHT_PIXELS/2});

    hpBarBackground.setSize({HP_WIDTH_PIXELS, HP_HEIGHT_PIXELS});
    hpBarBackground.setFillColor(sf::Color::Black);
    hpBarBackground.setOrigin({HP_WIDTH_PIXELS / 2, HP_HEIGHT_PIXELS / 2});
    

    damageBar.setSize({HP_WIDTH_PIXELS, HP_HEIGHT_PIXELS});
    damageBar.setFillColor(damageColour);//sf::Color(255, 0, 0, 180));
    damageBar.setOrigin({HP_WIDTH_PIXELS / 2, HP_HEIGHT_PIXELS / 2});
}

// use interpolation to make the HP bar decrease fast initially then slow down as it reaches target HP
void Hud::InterpolateHpBar() {
    //currentHpWidth = currentHpWidth - static_cast<int>(std::ceil((currentHpWidth-targetHpWidth)*INTERPOLATION_FACTOR));
    currentHpWidth = std::max(1.f,targetHpWidth);
    
    if (damageHpWidth > targetHpWidth) {
        float diff = damageHpWidth - targetHpWidth;
        damageHpWidth -= std::max(diff * INTERPOLATION_FACTOR * 0.65f, 0.15f); // min amount to move
    } else {
        damageHpWidth = targetHpWidth; // If no damage, keep same size
    }
}

// current = current - difference*rate
void Hud::Update(int currHp, sf::FloatRect globalBounds) {
    targetHpWidth = (static_cast<float>(currHp) / maxHp) * HP_WIDTH_PIXELS;
    InterpolateHpBar(); 
    hpBar.setSize({currentHpWidth, HP_HEIGHT_PIXELS});
    hpBar.setPosition({std::round(globalBounds.position.x + globalBounds.size.x/2), std::round(globalBounds.position.y - HP_Y_OFFSET)});
    damageBar.setSize({damageHpWidth, HP_HEIGHT_PIXELS});
    damageBar.setPosition({std::round(hpBar.getPosition().x), std::round(hpBar.getPosition().y)});
    
    UpdateSegments(maxHp);
    hpBarBackground.setPosition({std::round(hpBar.getPosition().x), std::round(hpBar.getPosition().y)});
}

void Hud::UpdateSegments(int maxHealth) {
    segmentLines.clear();
    float barLeft = hpBar.getPosition().x - (HP_WIDTH_PIXELS / 2); // starting position of left side of hp bar. - origin offset
    for (int hp = 100; hp < maxHealth; hp += 100) {
        float xPosition = (hp / static_cast<float>(maxHealth)) * HP_WIDTH_PIXELS;
        if (xPosition > targetHpWidth) break;
        sf::RectangleShape line;
        if (hp % 1000 == 0) {
            line.setSize({SEGMENT_LINE_THICKNESS * 2, HP_HEIGHT_PIXELS}); // every 1000 hp a more bold line and full height
        } else {
            line.setSize({SEGMENT_LINE_THICKNESS, HP_HEIGHT_PIXELS * 0.5f}); // for 100 hp, only half height and thinner
        }
        line.setFillColor(sf::Color::Black);
        line.setOrigin({0, 0});
        line.setPosition({std::round(barLeft + xPosition), std::round(hpBar.getPosition().y - HP_HEIGHT_PIXELS / 2)});
        segmentLines.push_back(line);
    }
    sf::RectangleShape topBorder;
    topBorder.setSize({HP_WIDTH_PIXELS, SEGMENT_LINE_THICKNESS});
    topBorder.setFillColor(sf::Color::Black);
    topBorder.setPosition({hpBar.getPosition().x - (HP_WIDTH_PIXELS / 2), hpBar.getPosition().y - (HP_HEIGHT_PIXELS / 2) - SEGMENT_LINE_THICKNESS});
    segmentLines.push_back(topBorder);

    sf::RectangleShape bottomBorder;
    bottomBorder.setSize({HP_WIDTH_PIXELS, SEGMENT_LINE_THICKNESS});
    bottomBorder.setFillColor(sf::Color::Black);                                                                // positioned at bottom of bar so doesn't
    bottomBorder.setPosition({hpBar.getPosition().x - (HP_WIDTH_PIXELS / 2), hpBar.getPosition().y + (HP_HEIGHT_PIXELS / 2)}); // need extra segment line offset 
    segmentLines.push_back(bottomBorder);

    sf::RectangleShape leftBorder;
    leftBorder.setSize({SEGMENT_LINE_THICKNESS, HP_HEIGHT_PIXELS + 2 * SEGMENT_LINE_THICKNESS});
    leftBorder.setFillColor(sf::Color::Black);
    leftBorder.setPosition({hpBar.getPosition().x - (HP_WIDTH_PIXELS / 2) - SEGMENT_LINE_THICKNESS, hpBar.getPosition().y - (HP_HEIGHT_PIXELS / 2) - SEGMENT_LINE_THICKNESS});
    segmentLines.push_back(leftBorder);
  
    sf::RectangleShape rightBorder;
    rightBorder.setSize({SEGMENT_LINE_THICKNESS, HP_HEIGHT_PIXELS + 2 * SEGMENT_LINE_THICKNESS});
    rightBorder.setFillColor(sf::Color::Black);
    rightBorder.setPosition({hpBar.getPosition().x + (HP_WIDTH_PIXELS / 2), hpBar.getPosition().y - (HP_HEIGHT_PIXELS / 2) - SEGMENT_LINE_THICKNESS});
    segmentLines.push_back(rightBorder);

}

// render hp bar
void Hud::Draw(sf::RenderWindow& window) {
   window.draw(hpBar);
   for (const auto& line : segmentLines) {
        window.draw(line);
    }
   //HitboxDebugger::DrawGlobalRect(window, hpBar.getGlobalBounds());
}
