#include "weapons/Grenade.h"
#include "core/GameState.h"

static constexpr float MIRROR_NEG_Y_AXIS_BOUND = M_PI_2;
static constexpr float MIRROR_POS_Y_AXIS_BOUND =  -M_PI_2;

Grenade::Grenade(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized, sf::Vector2f targetPos, AnimData explosionAnimData, ExplosionData explosionData)
: Projectile(projectileData, position, normalized), 
normalized(normalized), 
targetPos(targetPos),
explosionAnimData(explosionAnimData), 
explosionData(explosionData), 
perp({0,0})
{
    sf::Vector2f perp;
    createsBlood = false;
    float radians = atan2(normalized.y, normalized.x);
    if(radians > MIRROR_NEG_Y_AXIS_BOUND || radians < MIRROR_POS_Y_AXIS_BOUND){ // FACING RIGHT
        perp = {-normalized.y, normalized.x};
    } else {
        perp = {normalized.y, -normalized.x};
    }
    sf::Vector2f diff = targetPos-position;
    distToTarget = diff.length();
    
    //// relation 10.f for 1200
    initUp = distToTarget*(1/105.f);
    velocity += perp * initUp;

}

bool Grenade::ReachedDest() {
    return distTravelled > distToTarget;
}


void Grenade::UpdatePosition(float deltaTime) {
    sf::Vector2f perp;
    float radians = atan2(normalized.y, normalized.x);
    if(radians > MIRROR_NEG_Y_AXIS_BOUND || radians < MIRROR_POS_Y_AXIS_BOUND){ // FACING RIGHT
        perp = {normalized.y, -normalized.x};
    } else {
        perp = {-normalized.y, normalized.x};
    }
    auto gravityInfluence = perp * gravity * deltaTime; // Apply gravity
    velocity += gravityInfluence;
    distTravelled += velocity.length();
    sprite.setPosition(GetPosition() + velocity + gravityInfluence);
    Rotate(velocity);
}

void Grenade::UpdateProjectileStatus(Character& character, GameState& state, 
    std::vector<std::unique_ptr<Projectile>>::iterator& it) {
    if(ReachedDest()){
        sf::Vector2f position = (*it)->GetPosition();
        state.aoe.emplace_back(std::make_unique<Explosion>(explosionAnimData, position, explosionData));
        it = state.projectiles.erase(it);
    } else {
        ++it;
    }
}

void Grenade::ActivatePostTargetReached(GameState& state, std::vector<std::unique_ptr<Projectile>>::iterator& it){
    sf::Vector2f position = (*it)->GetPosition();
    state.aoe.emplace_back(std::make_unique<Explosion>(explosionAnimData, position, explosionData));
    it = state.projectiles.erase(it);
}