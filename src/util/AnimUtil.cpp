#include <SFML/Graphics.hpp>
#include <iostream>
#include "util/AnimUtil.h"
#include "util/TextureUtil.h"

sf::Texture* AnimUtil::playerTexture = TextureUtil::GetTexture("../assets/textures/characters/bunny_white_update.png");
sf::Texture* AnimUtil::dungeonTexture = TextureUtil::GetTexture("../assets/textures/tilesheet.png");
sf::Texture* AnimUtil::bloodTexture = TextureUtil::GetTexture("../assets/textures/fx/blood_atlas.png");
sf::Texture* AnimUtil::weaponsTexture = TextureUtil::GetTexture("../assets/textures/weapons/weapons.png");
sf::Texture* AnimUtil::projectilesTexture = TextureUtil::GetTexture("../assets/textures/weapons/projectiles_atlas.png");
sf::Texture* AnimUtil::explosionTexture = TextureUtil::GetTexture("../assets/textures/fx/explosion.png");
sf::Texture* AnimUtil::explosionUpgradedTexture = TextureUtil::GetTexture("../assets/textures/fx/explosionAlt.png");
sf::Texture* AnimUtil::nukeTexture = TextureUtil::GetTexture("../assets/textures/fx/nuke.png");
sf::Texture* AnimUtil::statusEffectsTexture = TextureUtil::GetTexture("../assets/textures/fx/status_effects.png");
sf::Texture* AnimUtil::onFireTexture = TextureUtil::GetTexture("../assets/textures/fx/flame_effect_alpha.png");
sf::Texture* AnimUtil::onFireUpgradedTexture = TextureUtil::GetTexture("../assets/textures/fx/flame_effect_upgraded.png");


// PLAYER
const AnimData AnimUtil::PlayerAnim::stand = {playerTexture, sf::IntRect({0,10},{70,73}), 84, 0, 12, .1f, 0.f};
const AnimData AnimUtil::PlayerAnim::walk = {playerTexture, sf::IntRect({0,373},{70,73}), 84, 0, 8, .1f, 0.f};
const AnimData AnimUtil::PlayerAnim::shootStand = {playerTexture, sf::IntRect({756,637},{70,73}), 84, 0, 3, .1f, 0.f};
const AnimData AnimUtil::PlayerAnim::shootWalk = {playerTexture, sf::IntRect({0,465},{70,73}), 84, 0, 8, .1f, 0.f};

// MONSTERS

const AnimData AnimUtil::BigDemonAnim::walk = {dungeonTexture, sf::IntRect({148,432},{24,32}), 32, 0, 4, .1f, 0.f};
const AnimData AnimUtil::SmallDemonAnim::walk = {dungeonTexture, sf::IntRect({432,272},{16,24}), 16, 0, 4, .05f, 0.f};
const AnimData AnimUtil::SlugAnim::walk = {dungeonTexture, sf::IntRect({368,374},{16,18}), 16, 0, 4, .1f, 0.f};


// BLOOD
const AnimData AnimUtil::BloodAnim::spray1 = {bloodTexture, sf::IntRect({32, 108},{96,112}), 112, 0, 9, .05f, 0.f};
const AnimData AnimUtil::BloodAnim::spray2 = {bloodTexture, sf::IntRect({32, 300},{96,96}), 112, 0, 10, .05f, 0.f};
const AnimData AnimUtil::BloodAnim::spray3 = {bloodTexture, sf::IntRect({32, 396},{96,96}), 112, 0, 8, .05f, 0.f};
const AnimData AnimUtil::BloodAnim::spray5 = {bloodTexture, sf::IntRect({0, 570},{112,96}), 109, 0, 14, .02f, 0.f};
const AnimData AnimUtil::BloodAnim::spray4 = {bloodTexture, sf::IntRect({32, 500},{96,75}), 112, 0, 8, .05f, 0.f};
const AnimData AnimUtil::BloodAnim::spray6 = {bloodTexture, sf::IntRect({0, 760},{112,96}), 109, 0, 10, .05f, 0.f};
const AnimData AnimUtil::BloodAnim::ground = {bloodTexture, sf::IntRect({0, 850},{210,250}), 0, 0, 9, .01f, 0.f};
const AnimData AnimUtil::BloodAnim::leftFootprint = {bloodTexture, sf::IntRect({0, 0},{35,45}), 0, 0, 1, .05f, 0.f};
const AnimData AnimUtil::BloodAnim::rightFootprint = {bloodTexture, sf::IntRect({35, 0},{35,45}), 0, 0, 1, .05f, 0.f};



const AnimData AnimUtil::WeaponAnim::ak47 = {weaponsTexture, sf::IntRect({192, 32},{64,28}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::ak47Upgraded = {{256, 32},{64,28}};

const AnimData AnimUtil::WeaponAnim::famas = {weaponsTexture, sf::IntRect({160, 224},{64,32}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::famasUpgraded = {{224, 224},{64,32}};

const AnimData AnimUtil::WeaponAnim::barrett50 = {weaponsTexture, sf::IntRect({0, 160},{96,35}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::barrett50Upgraded = {{96,160},{96,35}};
//RPG
const AnimData AnimUtil::WeaponAnim::rpg = {weaponsTexture, sf::IntRect({64, 196},{90,20}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::rpgReloadRect({64, 196},{60,20});
const sf::IntRect AnimUtil::WeaponAnim::rpgLoadedRect({64, 196},{90,20});
const sf::IntRect AnimUtil::WeaponAnim::rpgUpgradedReloadRect({160, 196},{60,20});
const sf::IntRect AnimUtil::WeaponAnim::rpgUpgradedLoadedRect({160, 196},{100,20});


//p90 6,37 TL 51x21,  70 x, same y etc.
const AnimData AnimUtil::WeaponAnim::p90 = {weaponsTexture, sf::IntRect({6, 37},{51,21}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::p90Upgraded = {{70, 37},{51,21}};
//ScarH 0, 67 y 64x25 64x same
const AnimData AnimUtil::WeaponAnim::scarH = {weaponsTexture, sf::IntRect({0, 67},{64,25}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::scarHUpgraded = {{64, 67},{64,25}};
//m4a1 130,67y 59x25 194xsame
const AnimData AnimUtil::WeaponAnim::m4a1 = {weaponsTexture, sf::IntRect({130, 67},{59,25}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::m4a1Upgraded = {{194, 67},{59,25}};
//m1014 0,103y 63x17 65x same
const AnimData AnimUtil::WeaponAnim::m1014 = {weaponsTexture, sf::IntRect({0, 103},{63,17}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::m1014Upgraded = {{65, 103},{63,17}};
//ump45 132,99y 55x26 196x same
const AnimData AnimUtil::WeaponAnim::ump45 = {weaponsTexture, sf::IntRect({132, 99},{55,26}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::ump45Upgraded = {{196, 99},{55,26}};
//uzi 160, 134 32x20 192 same
const AnimData AnimUtil::WeaponAnim::uzi = {weaponsTexture, sf::IntRect({160, 134},{32,20}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::uziUpgraded = {{192, 134},{32,20}};
//flamethrower 0,121y  75x40 80x same
const AnimData AnimUtil::WeaponAnim::flamethrower = {weaponsTexture, sf::IntRect({0, 121},{75,40}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::flamethrowerUpgraded = {{80, 121},{75,40}};
// m32 grenade launcher 196,166y 54x20 260 same
const AnimData AnimUtil::WeaponAnim::m32 = {weaponsTexture, sf::IntRect({196, 166},{54,20}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::m32Upgraded = {{260, 166},{54,20}};
// m240 lmg 0,224 80x32 80 same
const AnimData AnimUtil::WeaponAnim::m240 = {weaponsTexture, sf::IntRect({0, 224},{80,32}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::m240Upgraded = {{80, 224},{80,32}};
// m9 288,229y 32x22 321 same
const AnimData AnimUtil::WeaponAnim::m9 = {weaponsTexture, sf::IntRect({288, 229},{32,22}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::m9Upgraded = {{321, 229},{32,22}};
// magnum 0,198y 32x22 32 same
const AnimData AnimUtil::WeaponAnim::magnum = {weaponsTexture, sf::IntRect({0, 198},{32,22}), 0, 0, 1, .2f, 0.f};
const sf::IntRect AnimUtil::WeaponAnim::magnumUpgraded = {{32, 198},{32,22}};



const AnimData AnimUtil::WeaponFxAnim::muzzleFlash1 = {projectilesTexture, sf::IntRect({1272,13},{82,52}), 82, 0, 3, .1f, 0.f};
const AnimData AnimUtil::WeaponFxAnim::muzzleFlash2 = {projectilesTexture, sf::IntRect({1273,96},{83,47}), 83, 0, 3, .1f, 0.f};
const AnimData AnimUtil::WeaponFxAnim::muzzleFlash3 = {projectilesTexture, sf::IntRect({1271,171},{85,53}), 85, 0, 3, .1f, 0.f};
const AnimData AnimUtil::WeaponFxAnim::muzzleFlash4 = {projectilesTexture, sf::IntRect({1268,247},{82,62}), 82, 0, 3, .1f, 0.f};
const AnimData AnimUtil::WeaponFxAnim::muzzleFlash5 = {projectilesTexture, sf::IntRect({1273,326},{82,58}), 82, 0, 3, .1f, 0.f};

const SubRectData AnimUtil::WeaponFxAnim::rpgBackfire =  {
{
    sf::IntRect{{1270,9},{65,56}},
    sf::IntRect{{1354,9},{63,50}},
    sf::IntRect{{1435,11},{58,53}}
}, .02f
};

const SubRectData AnimUtil::WeaponFxAnim::rpgSmoke = {
    {
    sf::IntRect{{541,632},{50,53}},
    sf::IntRect{{671,632},{50,53}},
    sf::IntRect{{798,632},{50,53}},
    sf::IntRect{{928,626},{50,62}},
    sf::IntRect{{1049,623},{60,70}},
    sf::IntRect{{1174,617},{67,73}},
    sf::IntRect{{1303,623},{67,74}},
    
    }, .05f 
};

const AnimData AnimUtil::WeaponFxAnim::explosion = {explosionTexture, sf::IntRect({0,0},{192,192}), 192, 0, 15, .05f, 0.f, 5, 192};
const AnimData AnimUtil::WeaponFxAnim::nuke = {nukeTexture, sf::IntRect({0,0},{256,128}), 256, 0, 12, .05f, 0.f, 3};
const AnimData AnimUtil::WeaponFxAnim::explosionUpgraded = {explosionUpgradedTexture, sf::IntRect({0,0},{256,256}), 256, 0, 30, .05f, 0.f, 3, 256};


const AnimData AnimUtil::StatusFxAnim::frozen = {statusEffectsTexture, sf::IntRect({0,0},{100,91}), 100, 0, 10, .3f};
const AnimData AnimUtil::StatusFxAnim::paralyze = {statusEffectsTexture, sf::IntRect({0,100},{128,128}), 128, 0, 8, .06f, 0.f};
const AnimData AnimUtil::StatusFxAnim::onFire = {onFireTexture, sf::IntRect({0,0},{100,100}), 100, 0, 55, .02f, 0.f, 5, 100};
const AnimData AnimUtil::StatusFxAnim::onFireUpgraded = {onFireUpgradedTexture, sf::IntRect({0,0},{100,100}), 100, 0, 55, .02f, 0.f, 5, 100};

const AnimData AnimUtil::ProjectileAnim::smallBulletReg = {projectilesTexture, sf::IntRect({90,8},{9,7})};
const AnimData AnimUtil::ProjectileAnim::medBulletReg = {projectilesTexture, sf::IntRect({109,7},{19,9})};
const AnimData AnimUtil::ProjectileAnim::shotgunPelletReg = {projectilesTexture, sf::IntRect({177,7},{16,10})};
const AnimData AnimUtil::ProjectileAnim::rpgRocketReg = {projectilesTexture, sf::IntRect({4,4},{36,17})};
const AnimData AnimUtil::ProjectileAnim::rpgRocketUpgrade = {projectilesTexture, sf::IntRect({41,4},{40,20})};
const AnimData AnimUtil::ProjectileAnim::barrett50BulletReg = {projectilesTexture, sf::IntRect({135,7},{34,10})};
const AnimData AnimUtil::ProjectileAnim::barrett50BulletUpgrade = {projectilesTexture, sf::IntRect({442,540},{158,41})};

const AnimData AnimUtil::ProjectileAnim::redLaser= {projectilesTexture, sf::IntRect({12,46},{126,71})};
const AnimData AnimUtil::ProjectileAnim::yellowLaser= {projectilesTexture, sf::IntRect({192,47},{124,71})};
const AnimData AnimUtil::ProjectileAnim::orangeLaser= {projectilesTexture, sf::IntRect({365,45},{121,70})};
const AnimData AnimUtil::ProjectileAnim::purpleLaser= {projectilesTexture, sf::IntRect({553,47},{121,70})};
const AnimData AnimUtil::ProjectileAnim::whiteLaser= {projectilesTexture, sf::IntRect({729,49},{109,58})};

const AnimData AnimUtil::ProjectileAnim::lightningBullet = {projectilesTexture, sf::IntRect({89,137},{149,37}), 192, 0, 5, .05f, 0.f};
const AnimData AnimUtil::ProjectileAnim::iceBullet = {projectilesTexture, sf::IntRect({0,595},{41,24}), 49, 0, 10, .1f, 0.f};
const AnimData AnimUtil::ProjectileAnim::atomBullet = {projectilesTexture, sf::IntRect({308,803},{62,62}), 102, 0, 12, .05f, 0.f, 3, 102};
const AnimData AnimUtil::ProjectileAnim::waveBullet = {projectilesTexture, sf::IntRect({608,780},{97,153}), 210, 0, 8, .1f, 0.f, 3, 200};
const AnimData AnimUtil::ProjectileAnim::electricBullet = {projectilesTexture, sf::IntRect({0,1434},{220,66}), 260, 0, 4, .06f, 0.f};

                                                                                // 0, 1434 y  220 x 66, 260 x gap   


// 
//614x614 total texture 97x153 608,780 
// 2nd  97x153,         818x start, 780 again = 210
//984 top of second row 200 y diff


//307x410 288,783 // total 102x each 102.5y  // 370,804 other side // 866 y is bottom
//         // get ready for some projectile coordinates :D

//         // rpg rocket 4,4  36x17
//         // rpg upgraded rocket 41,2, 34x18
//         // small pistol bullet 90,8, 9x7
//         // med bullet, 109,7, 19x9
//         // long bullet, 135,7, 34x10
//         // shotgun bullet, 177,7 16x10 
//         // grenade launcher ammo, 0,23 18x12
//         // grenade launcher upgraded ammo, 29,24 , 17x10
//         // frag 2, 198 22x21
//         // stun grenade 3,225 13x23
//         // smoke grenade 23,225 13x23
//         // laser red 12,46 126x71 hit 21,70 104x20
//         // laser yellow 192,47 124x71 hit 196,72 same
//         // laser orange 365,45 121x70 hit 374,72 same
//         // laser purple 553,47, 121x70 hit 558,72 same
//         // laser white 729, 49, 109x58 hit 732, 72 same
//         // green sniper 158x41 442,540

//         // laser pistol bullet 1070, 69 118x103, hit 1107,102 42x30
//         // black outline laser 1073,188 119x65, hit 1095,204 83x33 
//         // flame bullet 1058,312 146x69 hit 1064,333 117x31

//         // Lightning Animations
//         // 237x54 18,790
//         // 237x53 18, 847
//         // 231x57 21,903
//         // 227x67 22,959
//         // 226x64 21, 1026
//         // 237x55 20, 1092
//         // 228x54 22, 1162
//         // 230x57 22, 1218

// // Commonly used function for updating a sprite animation given delta time and animation
// // returns TRUE if it has reached it's last frame.  This can be used with iterator to remove in place after a
// // sequence ends
// // NOTE: this tracks a deltaTimeSum to track time passed since loading a frame
// //  animSpeed determines how long a single frame will be rendered for in seconds, multiple passes will render the same frame.
bool AnimUtil::UpdateSpriteAnim(sf::Sprite& sprite, AnimData& animData, float deltaTime) {
    // if deltaTime >= animSpeed, ready for next frame
    if(animData.deltaTimeSum >= animData.animSpeed) {
        // update the sub rectangle of the texture to point to the next frame
        int textureCoordsPosX = animData.textureFrame.position.x + (animData.currFrame % animData.totalFrames) * animData.frameSpacing;  // 0 mod 4 = 0, 4 mod 4 = 0
        sprite.setTextureRect(sf::IntRect({textureCoordsPosX, animData.textureFrame.position.y}, animData.textureFrame.size));
        animData.currFrame++;
        // if the last frame of a sequence has been rendered, loop back to the first one
        if (animData.currFrame >= animData.totalFrames) {
            animData.currFrame = 0; 
            return true;
        }
        animData.deltaTimeSum = 0.f;
        
    }
    animData.deltaTimeSum += deltaTime;
    return false;
}

bool AnimUtil::UpdateSpriteXYAnim(sf::Sprite& sprite, AnimData& animData, float deltaTime){
        // if deltaTime >= animSpeed, ready for next frame
        if(animData.deltaTimeSum >= animData.animSpeed) {
            // update the sub rectangle of the texture to point to the next frame
            // frame spacing is inclusive of frame, amount to increment x each frame
            int posX = animData.textureFrame.position.x + (animData.currFrame % animData.rowLength) * animData.frameSpacing;  // 0 mod 4 = 0, 4 mod 4 = 0
            int posY = animData.textureFrame.position.y + (animData.currFrame/animData.rowLength) * animData.yFrameSpacing; // get the Y
            sprite.setTextureRect(sf::IntRect({posX, posY}, animData.textureFrame.size));
            animData.currFrame++;
            // if the last frame of a sequence has been rendered, loop back to the first one
            if (animData.currFrame >= animData.totalFrames) {
                animData.currFrame = 0; 
                return true; // return true when done
            }
            animData.deltaTimeSum = 0.f;
        }
        animData.deltaTimeSum += deltaTime;
        return false;
}

// update a subrectangle vector, and hide when it's done 
bool AnimUtil::UpdateSubRect(SubRectData& subRectData, float deltaTime) {
    if(subRectData.hide) return true;
    if(subRectData.deltaTimeSum >= subRectData.animSpeed) {
        subRectData.currFrame++;
        if(subRectData.currFrame >= subRectData.frameSequence.size()){
            subRectData.currFrame = 0;
            if(subRectData.repeat){
                return false;
            } else {
                subRectData.hide = true;
                return true;
            }
        }
        subRectData.deltaTimeSum = 0.f;
    }
    subRectData.deltaTimeSum += deltaTime;
    return false;
}