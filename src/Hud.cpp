#include "Hud.h"

static constexpr float HP_WIDTH_PIXELS = 60.f;
static constexpr float HP_HEIGHT_PIXELS = 6.f;
static constexpr float INTERPOLATION_FACTOR = .1f;
static constexpr float HP_Y_OFFSET = 10.f;

// Default constructor, create HP bar
Hud::Hud() : currentHpWidth(HP_WIDTH_PIXELS), targetHpWidth(HP_WIDTH_PIXELS){
    hpBar.setFillColor(sf::Color::Green);
    hpBar.setOrigin({HP_WIDTH_PIXELS/2, HP_HEIGHT_PIXELS/2});
}

// use interpolation to make the HP bar decrease fast initially then slow down as it reaches target HP
void Hud::InterpolateHpBar() {
    currentHpWidth = currentHpWidth - (currentHpWidth-targetHpWidth)*INTERPOLATION_FACTOR;
}

// current = current - difference*rate
// will update every frame to make smoother
void Hud::Update(int health, sf::FloatRect globalBounds) {
    // target width =  ratio of hp left * width of total hp
    targetHpWidth = (health / 100.f) * HP_WIDTH_PIXELS;
    InterpolateHpBar(); 
    hpBar.setSize({currentHpWidth, HP_HEIGHT_PIXELS});
    // if (health < 15) {
    //     hpBar.setFillColor(sf::Color::Red);
    // }
    hpBar.setPosition({globalBounds.position.x + globalBounds.size.x/2, globalBounds.position.y - HP_Y_OFFSET});
}

// render hp bar
void Hud::Draw(sf::RenderWindow& window) {
    window.draw(hpBar);
}
