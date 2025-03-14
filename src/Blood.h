#pragma once
#include <SFML/Graphics.hpp>
#include "AnimUtil.h"
#include "BatchRenderer.h"

class Footprint;
class GroundBlood;

class Blood : public sf::Transformable{

protected:
    //sf::Sprite sprite;
    AnimData animData;
    sf::Vector2f position;
    sf::Color colour;


public:
    static sf::Texture* texture;

    // delete later
    sf::Texture* tempTexture;
    /// delete above


    Blood(AnimData animData, sf::Vector2f position);
    virtual void SetRotation(sf::Vector2f incomingProjectilePos);
    static void Update(std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, float deltaTime); // update new prints and footprint transparency
    static AnimData GetNextSprayAnim();
    static void RenderBlood(std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, 
        std::vector<Footprint>& footprints, BatchRenderer& batchRenderer, sf::RenderWindow& window); // render footprint vector
    //void Draw(sf::RenderWindow& window);
    void SetPosition(sf::Vector2f position);
    static void UpdateProjectileBlood(sf::Vector2f incomingProjectilePos, sf::FloatRect characterGlobalBounds, 
        std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood);

    //inline sf::Sprite& GetSprite() { return sprite;}
    inline sf::Color& GetColour() {return colour;}
    inline sf::Texture* GetTexture() {return tempTexture;}//texture;}
    inline AnimData& GetAnimData() {return animData;}
    inline sf::Vector2f GetPosition() {return getPosition();}
    //inline sf::FloatRect GetGlobalBounds() {return getGlobalBounds();}
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