#include "Projectile.h"

class Grenade : public Projectile {


public:
    AnimData explosionAnimData;
    ExplosionData explosionData;
    sf::Vector2f normalized;
    sf::Vector2f targetPos;
    sf::Vector2f perp;


    float gravity = 30.f; // Pixels per second squared
    float initUp = 0.f;
    float distToTarget = 0.f;
    float distTravelled = 0.f;
    
    
    Grenade(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized, sf::Vector2f targetPos, AnimData explosionAnimData, ExplosionData explosionData);
    void UpdatePosition(float deltaTime) override;
    void UpdateProjectileStatus(Character& character, GameState& state, 
        std::vector<std::unique_ptr<Projectile>>::iterator& it) override;
    bool ReachedDest() override;
    void ActivatePostTargetReached(GameState& state, std::vector<std::unique_ptr<Projectile>>::iterator& it) override;

    };