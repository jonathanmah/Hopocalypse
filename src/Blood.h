#pragma once
#include <SFML/Graphics.hpp>
#include "AnimUtil.h"
#include "BatchRenderer.h"
#include "RenderUtil.h"


class GameState;
class AnimUtil;
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
    //void Draw(sf::RenderWindow& window);
    void SetPosition(sf::Vector2f position);
    static void CreateProjectileBlood(sf::Vector2f incomingProjectilePos, sf::FloatRect characterGlobalBounds, 
        std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood);
    bool UpdateBloodSprayAnim(float deltaTime);
    void CachePositionVertices() {cachedVertices = RenderUtil::CalculatePositionVertices(animData.textureFrame, getTransform());}
    inline std::array<sf::Vector2f, 4> GetCachedVertices() {return cachedVertices;}


    //inline sf::Sprite& GetSprite() { return sprite;}
    inline sf::Color& GetColour() {return colour;}
    inline sf::Texture* GetTexture() {return texture;}
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