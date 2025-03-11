#include <SFML/Graphics.hpp>
#include "AnimUtil.h"
#include "TextureUtil.h"
#include <iostream>

namespace AnimUtil {
    namespace PlayerAnim {
        namespace PrincessAnim {
            const std::string texturePath = "../assets/textures/characters/MiniPrincess.png";
            const AnimData walkAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({8,48},{16,16}), 32, 0, 6, .1f, 0.f};
            const AnimData standAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({8,16},{16,16}), 32, 0, 4, .1f, 0.f};
            const AnimData deathAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({8,144},{24,16}), 32, 0, 5, .1f, 0.f};
            const AnimData hitAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({8,112},{16,16}), 32, 0, 3, .1f, 0.f};
        }
    }

    namespace MonsterAnim {
        const std::string texturePath = "../assets/textures/tilesheet.png";
        namespace BigDemonAnim {
            const AnimData walkAnim = { TextureUtil::LoadTexture(texturePath), sf::IntRect({20+32*4,27*16}, {24,32}),32, 0, 4, .1f, 0.f};
            const AnimData standAnim = { TextureUtil::LoadTexture(texturePath), sf::IntRect({20,27*16}, {24,32}), 32, 0, 4, .1f, 0.f};
        }
        namespace SmallDemonAnim {
            const AnimData walkAnim = { TextureUtil::LoadTexture(texturePath), sf::IntRect({27*16,17*16},{16,24}), 16, 0, 4, .05f, 0.f};
            const AnimData standAnim = { TextureUtil::LoadTexture(texturePath), sf::IntRect({27*16,17*16},{16,24}), 16, 0, 4, .05f, 0.f};
        }
        namespace BigSlugAnim {
            const AnimData walkAnim = { TextureUtil::LoadTexture(texturePath), sf::IntRect({23*16,23*16+6},{16,18}), 16, 0, 4, .1f, 0.f};
            const AnimData& standAnim = walkAnim;
        }
    }

    namespace ProjectileAnim {
        const std::string texturePath = "../assets/textures/orange_bullets.png";
        namespace RegularBullet {
            const AnimData anim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({22*16, 3*16},{16,16}), 0, 0, 1, .1f, 0.f};
         // 12 x 
        }
    }

    namespace WeaponAnim { // tile sheet is 20x16 16px
        const std::string texturePath = "../assets/textures/weapons.png"; 
        const AnimData AK47anim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({12*16, 2*16},{4*16,28}), 0, 0, 1, .2f, 0.f};
        const AnimData FAMASanim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({8*16, 14*16},{4*16,3*16}), 0, 0, 1, .2f, 0.f};
        const AnimData Barrett50anim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({0, 10*16-8},{6*16,3*16}), 0, 0, 1, .2f, 0.f};
        const AnimData RPGanim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({12*16, 12*16-8},{6*16,2*16}), 0, 0, 1, .2f, 0.f};
        const AnimData RPGrocketAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({12*16, 8},{2*16,1*16}), 0, 0, 1, .2f, 0.f};
        const sf::IntRect RPGreloadRect({12*16, 12*16-8},{4*16,2*16});
        const sf::IntRect RPGloadedRect({12*16, 12*16-8},{6*16,2*16});
    }

    namespace BloodAnim {
        const std::string texturePath = "../assets/textures/blood.png";
        const float speed = .02f;
        const AnimData splatAnim1 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({1*16, 6*16},{6*16,7*16}), 7*16, 0, 9, speed, 0.f};
        const AnimData splatAnim2 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({10, 280},{6*16,6*16}), 7*16, 0, 10, speed, 0.f};
        const AnimData splatAnim3 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({1*16, 23*16},{6*16,7*16}), 7*16, 0, 8, speed, 0.f};
        const AnimData splatAnim4 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({0, 29*16},{6*16,6*16}), 7*16, 0, 8, speed, 0.f};
        const AnimData splatAnim5 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({0, 550},{7*16,7*16}), 110, 0, 14, speed, 0.f};
        const AnimData splatAnim6 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({0, 740},{7*16,7*16}), 110, 0, 10, speed, 0.f};
        const AnimData grenadeSplatAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({105, 0},{110,6*16}), 7*16, 0, 7, speed, 0.f};
        const AnimData trailAnim = {TextureUtil::LoadTexture("../assets/textures/ground_blood.png"), sf::IntRect({3*16, 0},{28*16,28*16}), 28*16, 0, 9, .01f, 0.f};

        namespace FootprintAnim {
            const std::string texturePath = "../assets/textures/footprints.png";
            const AnimData playerLeft = {TextureUtil::LoadTexture(texturePath), sf::IntRect({0, 0},{2*16,4*16}), 0, 0, 1, speed, 0.f};
            const AnimData playerRight = {TextureUtil::LoadTexture(texturePath), sf::IntRect({3*16, 0},{2*16,4*16}), 0, 0, 1, speed, 0.f};
        }
    }

    namespace HudAnim {
        const std::string texturePath = "../assets/textures/healthbar.png";
        const AnimData hpBar = {TextureUtil::LoadTexture(texturePath), sf::IntRect({3*16, 3*16},{3*16,1*16}), 0, 0, 6, .1f, 0.f};
    }
}

// Commonly used function for updating a sprite animation given delta time and animation
// returns TRUE if it has reached it's last frame.  This can be used with iterator to remove in place after a
// sequence ends

// NOTE: this tracks a deltaTimeSum to track time passed since loading a frame
//  animSpeed determines how long a single frame will be rendered for in seconds, multiple passes will render the same frame.
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
        
    };
    animData.deltaTimeSum += deltaTime;
    return false;
}