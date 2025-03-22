#pragma once

#include <SFML/Graphics.hpp>

class Blood;

struct AnimData {
    sf::Texture* texture = nullptr;
    sf::IntRect textureFrame; // a rect obj, position and size of a frame in texture
    int frameSpacing = 0;
    int currFrame = 0;
    int totalFrames = 1;
    float animSpeed = 0.1f;
    float deltaTimeSum = 0.f;
    int rowLength = 100;
    int yFrameSpacing = 0;
};
// 

struct SubRectData {
    std::vector<sf::IntRect> frameSequence;
    float animSpeed = 0.1f;
    bool repeat = false;
    bool hide = true;
    int currFrame = 0;
    float deltaTimeSum = 0.f;
};

class AnimUtil {

public:
    static void LoadTextures();
    // update a sequence horizontal
    static bool UpdateSpriteAnim(sf::Sprite& sprite, AnimData& animData, float deltaTime);
    // update a consistent sequence on a 2D sheet
    static bool UpdateSpriteXYAnim(sf::Sprite& sprite, AnimData& animData, float deltaTime);

    // animate a hardcoded sequence of subrects
    static bool UpdateSubRect(SubRectData& subRectData, float deltaTime);

    struct PlayerAnim {
        static const AnimData stand;
        static const AnimData walk;
        static const AnimData shootStand;
        static const AnimData shootWalk;
    };

    // MONSTERS

    struct BigDemonAnim {
        static const AnimData walk;
    };

    struct SmallDemonAnim {
        static const AnimData walk;
    };

    struct SlugAnim {
        static const AnimData walk;
    };


    struct BloodAnim {
        static const AnimData spray1;
        static const AnimData spray2;
        static const AnimData spray3;
        static const AnimData spray4;
        static const AnimData spray5;
        static const AnimData spray6;
        static const AnimData ground;
        static const AnimData leftFootprint;
        static const AnimData rightFootprint;
    };

      // WEAPONS

      struct WeaponAnim {
        static const AnimData ak47;
        static const sf::IntRect ak47Upgraded;
        static const AnimData famas;
        static const sf::IntRect famasUpgraded;
        static const AnimData barrett50;
        static const sf::IntRect barrett50Upgraded;
        static const AnimData rpg; // upgraded is loaded rect
        static const sf::IntRect rpgReloadRect;
        static const sf::IntRect rpgLoadedRect;
        static const sf::IntRect rpgUpgradedReloadRect;
        static const sf::IntRect rpgUpgradedLoadedRect;

        static const AnimData p90;
        static const sf::IntRect p90Upgraded;
        static const AnimData scarH;
        static const sf::IntRect scarHUpgraded;
        static const AnimData m4a1;
        static const sf::IntRect m4a1Upgraded;
        static const AnimData m1014;
        static const sf::IntRect m1014Upgraded;
        static const AnimData ump45;
        static const sf::IntRect ump45Upgraded;
        static const AnimData uzi;
        static const sf::IntRect uziUpgraded;
        static const AnimData flamethrower;
        static const sf::IntRect flamethrowerUpgraded;
        static const AnimData m32;
        static const sf::IntRect m32Upgraded;
        static const AnimData m240;
        static const sf::IntRect m240Upgraded;
        static const AnimData m9;
        static const sf::IntRect m9Upgraded;
        static const AnimData magnum;
        static const sf::IntRect magnumUpgraded;

        static const AnimData fragGrenade;
        static const AnimData stunGrenade;
        static const AnimData smokeGrenade;
    };

    struct WeaponFxAnim {
        static const AnimData muzzleFlash1;
        static const AnimData muzzleFlash2;
        static const AnimData muzzleFlash3;
        static const AnimData muzzleFlash4;
        static const AnimData muzzleFlash5;

        static const AnimData explosion;
        static const AnimData explosionAlt;
        static const AnimData nuke;
        static const SubRectData rpgBackfire;
        static const SubRectData rpgSmoke;
    };

    struct StatusFxAnim {
        static const AnimData frozen;
        static const AnimData paralyze;
    };

    struct ProjectileAnim {
        static const AnimData smallBulletReg;
        static const AnimData medBulletReg;
        static const AnimData barrett50BulletReg;
        static const AnimData barrett50BulletUpgrade;
        static const AnimData shotgunPelletReg;
        static const AnimData rpgRocketReg;
        static const AnimData rpgRocketUpgrade;
        static const AnimData grenadeLauncherRoundReg;
        static const AnimData grenadeLauncherRoundUpgrade;
        static const AnimData redLaser;
        static const AnimData yellowLaser;
        static const AnimData orangeLaser;
        static const AnimData purpleLaser;
        static const AnimData whiteLaser;
        static const AnimData smallRedLaser;
        static const AnimData famasFlameBullet;
        static const AnimData lightningBullet;
        static const AnimData iceBullet;
        static const AnimData atomBullet;
        static const AnimData waveBullet;
        static const AnimData electricBullet;

    };
};