#include <iostream>
#include "weapons/derived/Flamethrower.h"
#include "util/TextureUtil.h"
#include "util/RandomUtil.h"
#include "core/GameState.h"
#include "entities/monster/Monster.h"
#include "entities/effects/OnFire.h"

static const sf::Texture* PROJECTILE_TEXTURE = TextureUtil::GetTexture("../assets/textures/weapons/projectiles_atlas.png");
static const float FLAME_OFFSET_SCALAR = 10.f;
static const float INIT_FLAME_LIFETIME = .8f;

static const ProjectileData flamethrowerReg = {
    AnimUtil::ProjectileAnim::medBulletReg,
    20.f, // speed
    1.f, // damage
    1.f, // bullet scale
    1, // collateral count
};

Flamethrower::Flamethrower() : 
Weapon(AnimUtil::WeaponAnim::flamethrower, 
        flamethrowerReg,
        {
            1.f, //scale 
            0.f, // baseOffsetX
            24.f, // baseOffsetY
            0.f, // x offset from muzzle
            -2.8f, // y offset from muzzle (pre recoil translation)
            48.f, // muzzleOffsetscalar
            0.15f, // fireRate
            .15f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .30f, // spread offset max
            .05f, //  spread offset growth
            .015f, // spread offset decay
            .0f, // time left until gun returns back to original position after recoil
            .15f, // total time gun takes to return back
            5.f, // amount of offset, vector created using this scalar opposite point direction
            5.f, // shell offset x
            -5.f, // shell offset y
        }
    )
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 3, sprite.getLocalBounds().size.y / 2});
}

sf::Color randomFlameColour() {
    int r = 200 + RandomUtil::GetRandomInt(40,55);
    int g = 100 + RandomUtil::GetRandomInt(0,155);
    int b = 50;  
    int a = 15.f;
    return sf::Color(r, g, b, a);
}
sf::Color randomUpgradedFlameColour() {
    int r = 50;
    int g = 100 + RandomUtil::GetRandomInt(0,155);
    int b = 200 + RandomUtil::GetRandomInt(40,55);  
    int a = 15.f;
    return sf::Color(r, g, b, a);
}

std::vector<std::reference_wrapper<Monster>> Flamethrower::GetNeighbourMonsters(std::vector<std::unique_ptr<Monster>>& monsters) {
    std::vector<std::reference_wrapper<Monster>> neighbouring;
    for(auto& monsterPtr : monsters) {
        if((monsterPtr->GetPosition()-GetPosition()).length() < 650.f){
            neighbouring.push_back(*monsterPtr);
        }
    }
    return neighbouring;
}

void Flamethrower::ShootFlame() {
    IncreaseSpread();
    for (int i = 0; i < 10; i++) { // Spawn multiple particles per frame
        sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
        sf::Vector2f velocity = adjustedNormal * RandomUtil::GetRandomFloat(500,800);//RandomUtil::GetRandomFloat(400,700); //500-800 original
        sf::Vector2f newPos = {muzzlePosition.x + RandomUtil::GetRandomInt(-5,5) , muzzlePosition.y + RandomUtil::GetRandomInt(-5,5)};
        sf::Color flameColour = !isUpgraded ? randomFlameColour() : randomUpgradedFlameColour();
        Flame flame{newPos, velocity, INIT_FLAME_LIFETIME, 1.0f, flameColour};
        flames.push_back(flame);
    }
}

void Flamethrower::FlameUpdate(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime) {
    std::vector<std::reference_wrapper<Monster>> closeMonsters = GetNeighbourMonsters(monsters);

    int num = 0;

    for (int i = 0; i < flames.size(); ) {
        Flame& flame = flames[i];
        flame.position += flame.velocity * deltaTime;
        flame.lifetime -= deltaTime;
        flame.size *= 1.055f; //1.055 is good
        flame.colour.a = static_cast<u_int8_t>(RandomUtil::GetRandomInt(25,40) * (flame.lifetime)); // 15-25 is good
        if(flame.lifetime > 0.25f*INIT_FLAME_LIFETIME) {
            for (Monster& monster : closeMonsters) {
                if(monster.hitbox.contains(flame.position)){
                    if(!monster.onFire.FlamethrowerDamageOnCooldown()){
                        int damage = !isUpgraded ? 100 : 300; // UPDATE DAMAGE HERE
                        monster.TakeDamage(damage);
                        monster.onFire.SetFlameThrowerDamageCooldown(0.2f);
                    }
                    if(!monster.IsDead()){
                        monster.onFire.ApplyEffect(5.f);
                    }
                        
                }
            }
        }
        if (flame.lifetime <= 0) {
            flames.erase(flames.begin() + i);
        } else {
            i++;
        }
    }
}

void Flamethrower::Update(GameState& state, Player& player, sf::Vector2f mousePosGlobal, float deltaTime) {
    SetMousePosGlobal(mousePosGlobal);
    UpdateBase(player.GetPosition(), deltaTime);
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        ShootFlame();
    } else {
        DecreaseSpread();
    }
    FlameUpdate(state.monsters, deltaTime);
    weaponData.timeSinceShot += deltaTime;
 }

void Flamethrower::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::flamethrowerUpgraded);
    isUpgraded = true;
}

void Flamethrower::Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer) {
    window.draw(sprite);
    batchRenderer.SetFlameTriangles(flames, 10.f);
   //HitboxDebugger::DrawSpriteGlobalBoundsHitbox(window, sprite, sf::Color::Yellow);
   //HitboxDebugger::DrawSpriteOrigin(window, sprite, sf::Color::Cyan);
}
void Flamethrower::CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) {return;}