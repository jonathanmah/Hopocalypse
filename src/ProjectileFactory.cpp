#include "ProjectileFactory.h"


// problems
// normal from character to mouse pos has offset. tried to create projectile from muzzle
// creating direction from muzzle to mouse pos has bug if cursor close to character. 
// normal from character to cursor is good, use muzzle position for initial position.
// need to fix offset. add to global mouse pos the diff between 
std::unique_ptr<Projectile> ProjectileFactory::CreateProjectile(Weapon* weapon, 
    sf::Vector2f weaponOrigin, sf::Vector2f relative, sf::Vector2f mousePosGlobal) {

    sf::Vector2f muzzlePosition = weapon->GetMuzzlePosition(relative.normalized());
    sf::Vector2f adjustedNormal = (mousePosGlobal - weaponOrigin).normalized();

    if(auto ak47Ptr = dynamic_cast<AK47*>(weapon)){
        // handle ak47 logic
        // if upgraded pass muzzle position, adjusted normal, projectile data.
        return std::make_unique<Projectile>(Projectile(ak47Ptr->GetProjectileData(), muzzlePosition,
            adjustedNormal));
    } else if (auto famasPtr = dynamic_cast<FAMAS*>(weapon)){
        // handle famas
        // if upgraded
        return std::make_unique<Projectile>(Projectile(famasPtr->GetProjectileData(), muzzlePosition,
            adjustedNormal));
    } else if (auto barrett50Ptr = dynamic_cast<Barrett50*>(weapon)){
        // handle barrett
        // if upgraded
        return std::make_unique<Projectile>(Projectile(barrett50Ptr->GetProjectileData(), muzzlePosition,
            adjustedNormal));
    } else if (auto rpgPtr = dynamic_cast<RPG*>(weapon)) {
        // handle rpg
        // if upgraded
        return std::make_unique<RPGrocket>(RPGrocket(rpgPtr->GetProjectileData(), muzzlePosition,
            adjustedNormal));

    } else {
        return nullptr;
    }
}