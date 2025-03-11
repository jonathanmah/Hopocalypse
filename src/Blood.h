#pragma once
#include <SFML/Graphics.hpp>
#include "AnimUtil.h"

class Footprint;
class GroundBlood;

class Blood {

protected:
    sf::Sprite sprite;
    static sf::Texture texture;
    AnimData animData;
    sf::Vector2f position;
    float scale;

public:
    Blood(AnimData animData, sf::Vector2f position);
    virtual void SetRotation(sf::Vector2f incomingProjectilePos);
    static void Update(std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, float deltaTime); // update new prints and footprint transparency
    static AnimData GetNextSprayAnim();
    static void RenderBlood(std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, 
        std::vector<Footprint>& footprints, sf::RenderWindow& window); // render footprint vector
    void Draw(sf::RenderWindow& window);
    void SetPosition(sf::Vector2f position);
    static void UpdateProjectileBlood(sf::Vector2f incomingProjectilePos, sf::FloatRect characterGlobalBounds, 
        std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood);

    inline sf::Sprite& GetSprite() { return sprite;}
    inline AnimData& GetAnimData() {return animData;}
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
};

class GroundBlood : public Blood {

private:
    sf::CircleShape collider;

public:
    GroundBlood(AnimData animData, sf::Vector2f position);
    void UpdateGroundBloodAnim(float deltaTime);
    static bool HasGroundBloodCollision(const sf::FloatRect& globalBounds, std::vector<GroundBlood>& groundBlood);
    void RenderCollider(sf::RenderWindow& window);
    void SetRotation(sf::Vector2f incomingProjectilePos) override;
};