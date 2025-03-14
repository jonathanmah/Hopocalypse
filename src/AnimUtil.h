#pragma once

#include <SFML/Graphics.hpp>

struct AnimData {
    sf::Texture* texture;
    sf::IntRect textureFrame; // a rect obj, position and size of a frame in texture
    int frameSpacing; // space between each frame in the texture - can be vertical or horizontal
    int currFrame;
    int totalFrames;
    float animSpeed;
    float deltaTimeSum;
};

namespace AnimUtil {
    bool UpdateSpriteAnim(sf::Sprite& sprite, AnimData& animData, float deltaTime);

    namespace PlayerAnim {
        namespace PrincessAnim {
            extern const AnimData walkAnim;
            extern const AnimData standAnim;
            extern const AnimData deathAnim;
            extern const AnimData hitAnim;
        }
    }

    namespace MonsterAnim {
        namespace BigDemonAnim {
            extern const AnimData walkAnim;
            extern const AnimData standAnim;
        }
        namespace SmallDemonAnim{
            extern const AnimData walkAnim;
            extern const AnimData standAnim;
        }
        namespace BigSlugAnim {
            extern const AnimData walkAnim;
            extern const AnimData& standAnim;
        }
    }

    namespace ProjectileAnim {
        namespace RegularBullet {
            extern const AnimData anim;
        }
    }

    namespace WeaponAnim {
        extern const AnimData ak47Anim;
        extern const AnimData famasAnim;
        extern const AnimData barrett50Anim;
        extern const AnimData rpgAnim;
        extern const AnimData rpgRocketAnim;
        extern const sf::IntRect rpgReloadRect;
        extern const sf::IntRect rpgLoadedRect;
    }

    namespace BloodAnim {
        extern const AnimData sprayAnim1;
        extern const AnimData sprayAnim2;
        extern const AnimData sprayAnim3;
        extern const AnimData sprayAnim4;
        extern const AnimData sprayAnim5;
        extern const AnimData sprayAnim6;
        extern const AnimData grenadeSprayAnim;
        extern const AnimData groundAnim;
        extern const AnimData trailAnim;
        
        namespace FootprintAnim {
            extern const AnimData playerLeft;
            extern const AnimData playerRight;
        }
    }

    namespace HudAnim {
        extern const AnimData hpBar;
    }

    namespace MuzzleFlashAnim {
        extern const AnimData muzzleFlash1;
        extern const AnimData muzzleFlash2;
        extern const AnimData muzzleFlash3;
        extern const AnimData muzzleFlash4;
        extern const AnimData muzzleFlash5;
    }
}