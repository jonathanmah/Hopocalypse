#include <SFML/Graphics.hpp>
#include "AnimUtil.h"
#include "TextureUtil.h"
#include <iostream>

namespace AnimUtil {
    namespace PlayerAnim {
        
        // have an init texture atlases function to create textures for each
        // look up if texture pointer is much space, can leave anim data as is... 
        
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
        const std::string texturePath = "../assets/textures/weapons_updated.png"; 
        const AnimData ak47Anim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({12*16, 2*16},{4*16,28}), 0, 0, 1, .2f, 0.f};
        const AnimData famasAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({8*16, 14*16},{4*16,32}), 0, 0, 1, .2f, 0.f};
        const AnimData barrett50Anim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({0, 10*16},{6*16,35}), 0, 0, 1, .2f, 0.f};
        const AnimData rpgAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({196, 196},{90,20}), 0, 0, 1, .2f, 0.f};
        const AnimData rpgRocketAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({12*16, 8},{2*16,1*16}), 0, 0, 1, .2f, 0.f};
        const sf::IntRect rpgReloadRect({196, 196},{60,20});
        const sf::IntRect rpgLoadedRect({196, 196},{90,20});
    }

    namespace BloodAnim {
        const std::string texturePath = "../assets/textures/blood_atlas_update.png";
        //const float speed = 0.05f;
        const float speed = 0.05f;
        /// 128 wide 
        const AnimData sprayAnim1 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({32, 108},{96,112}), 112, 0, 9, speed, 0.f};
        const AnimData sprayAnim2 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({32, 300},{96,96}), 112, 0, 10, speed, 0.f};
        const AnimData sprayAnim3 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({32, 396},{96, 96}), 112, 0, 8, speed, 0.f};
        const AnimData sprayAnim4 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({32, 500},{96,75}), 112, 0, 8, speed, 0.f};
        const AnimData sprayAnim5 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({0, 570},{112,96}), 109, 0, 14, .02f, 0.f};
        const AnimData sprayAnim6 = {TextureUtil::LoadTexture(texturePath), sf::IntRect({0, 760},{112,96}), 109, 0, 10, speed, 0.f};
        //const AnimData grenadeSprayAnim = {TextureUtil::LoadTexture(texturePath), sf::IntRect({105, 0},{110,6*16}), 7*16, 0, 7, speed, 0.f};
        const AnimData groundAnim = {TextureUtil::LoadTexture("../assets/textures/blood_atlas_update.png"), sf::IntRect({0, 850},{210,250}), 0, 0, 9, .01f, 0.f};
        //const AnimData groundAnim = {TextureUtil::LoadTexture("../assets/textures/blood_atlas_update.png"), sf::IntRect({0, 850},{210,250}), 0, 0, 9, .01f, 0.f};
        //const AnimData trailAnim = {TextureUtil::LoadTexture("../assets/textures/ground_blood.png"), sf::IntRect({3*16, 0},{28*16,28*16}), 28*16, 0, 9, .01f, 0.f};

        // 850 y begin, 250 coord y, 210 x
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


    namespace MuzzleFlashAnim {
        const std::string muzzleFlashPath = "../assets/textures/muzzleflashcopy.png";
        // 192 wide
        //24 for first, 220 where second one starts exactly, 413 for third
        // 176 ends
        //220-360 is y
        const AnimData muzzleFlash1 = {TextureUtil::LoadTexture(muzzleFlashPath), sf::IntRect({0, 0},{80,54}), 80, 0, 4, .01f, 0.f};
        const AnimData muzzleFlash2 = {TextureUtil::LoadTexture(muzzleFlashPath), sf::IntRect({0, 80},{80,54}), 80, 0, 4, .01f, 0.f};
        const AnimData muzzleFlash3 = {TextureUtil::LoadTexture(muzzleFlashPath), sf::IntRect({0, 160},{80,54}), 80, 0, 4, .01f, 0.f};
        const AnimData muzzleFlash4 = {TextureUtil::LoadTexture(muzzleFlashPath), sf::IntRect({0, 240},{80,54}), 80, 0, 4, .01f, 0.f};
        const AnimData muzzleFlash5 = {TextureUtil::LoadTexture(muzzleFlashPath), sf::IntRect({0, 320},{80,54}), 80, 0, 4, .01f, 0.f};
    }
// 590 and 200
    namespace EffectsAnim {
        const std::string smokePath = "../assets/textures/smoke.png"; // 130,58
        const AnimData rpgSmoke = {TextureUtil::LoadTexture(smokePath), sf::IntRect({0, 116},{75,60}), 55, 0, 4, 1.f, 0.f};
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
        
    }
    animData.deltaTimeSum += deltaTime;
    return false;
}