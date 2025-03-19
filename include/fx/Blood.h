#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"
#include "util/RenderUtil.h"

class GameState;
class Footprint;
class GroundBlood;

class Blood : public sf::Transformable{

protected:
    AnimData animData;
    sf::Vector2f position;
    sf::Color colour;
    std::array<sf::Vector2f, 4> cachedVertices;
    

public:
    static sf::Texture* texture;

    Blood(AnimData animData, sf::Vector2f position);
    virtual void SetRotation(sf::Vector2f incomingProjectilePos);
    static void Update(GameState& state, float deltaTime); // update new prints and footprint transparency
    static AnimData GetNextSprayAnim();
    static void RenderBlood(GameState& state);
    void SetPosition(sf::Vector2f position);
    static void CreateProjectileBlood(sf::Vector2f incomingProjectilePos, sf::FloatRect characterGlobalBounds, 
        std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood);
    bool UpdateBloodSprayAnim(float deltaTime);
    void CachePositionVertices() {cachedVertices = RenderUtil::CalculatePositionVertices(animData.textureFrame, getTransform());}
    inline std::array<sf::Vector2f, 4> GetCachedVertices() {return cachedVertices;}
    inline sf::Color& GetColour() {return colour;}
    inline sf::Texture* GetTexture() {return texture;}
    inline AnimData& GetAnimData() {return animData;}
    inline sf::Vector2f GetPosition() {return getPosition();}
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