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
 
        extern const AnimData AK47anim;
        extern const AnimData FAMASanim;
        extern const AnimData Barrett50anim;
        extern const AnimData RPGanim;
        extern const AnimData RPGrocketAnim;
        extern const sf::IntRect RPGreloadRect;
        extern const sf::IntRect RPGloadedRect;
    }

    namespace BloodAnim {
        extern const AnimData splatAnim1;
        extern const AnimData splatAnim2;
        extern const AnimData splatAnim3;
        extern const AnimData splatAnim4;
        extern const AnimData splatAnim5;
        extern const AnimData splatAnim6;
        extern const AnimData grenadeSplat;
        extern const AnimData trailAnim;
        
        namespace FootprintAnim {
            extern const AnimData playerLeft;
            extern const AnimData playerRight;
        }
    }

    namespace HudAnim {
        extern const AnimData hpBar;
    }
}