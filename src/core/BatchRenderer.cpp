#include <iostream>
#include "core/BatchRenderer.h"
#include "weapons/Weapon.h"
#include "util/AnimUtil.h"
#include "entities/monster/derived/Zombie.h"
#include "entities/monster/derived/SmallDemon.h"
#include "entities/monster/derived/BigDemon.h"
#include "entities/monster/derived/Wolf.h"
#include "entities/Player.h"
#include "entities/Character.h"

BatchRenderer::BatchRenderer(sf::RenderWindow& window) : window(window), flameTriangles(sf::PrimitiveType::Triangles){
    triangles.reserve(200); // allocate for 200 sprites 
}

// no transformations to object, but animated, so texture frame coordinates and colour change but 
// global bounds never change
void BatchRenderer::AddStaticFrameToBatch(const sf::IntRect& textureFrame, std::array<sf::Vector2f, 4> cachedPosition, sf::Color colour) {
    triangles.emplace_back(sf::Vertex{cachedPosition[0], colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y)}}); // top left 
    triangles.emplace_back(sf::Vertex{cachedPosition[1], colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}}); // top right
    triangles.emplace_back(sf::Vertex{cachedPosition[2], colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}}); // bottom left
    
    triangles.emplace_back(sf::Vertex{cachedPosition[2], colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}}); // bottom left
    triangles.emplace_back(sf::Vertex{cachedPosition[3], colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}}); // bottom right
    triangles.emplace_back(sf::Vertex{cachedPosition[1], colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}}); // top right
}

void BatchRenderer::AddSpriteToBatch(const sf::Sprite& sprite, std::vector<sf::Vertex>& vertices) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sf::Transform transform = sprite.getTransform();
    sf::IntRect textureFrame{sprite.getTextureRect()};

    // get the four corners of sprite and transform them
    sf::Vector2f topLeft = transform.transformPoint({bounds.position.x, bounds.position.y});
    sf::Vector2f bottomLeft = transform.transformPoint({bounds.position.x, bounds.position.y+bounds.size.y});
    sf::Vector2f topRight = transform.transformPoint({bounds.position.x+bounds.size.x, bounds.position.y});
    sf::Vector2f bottomRight = transform.transformPoint({bounds.position.x+bounds.size.x, bounds.position.y+bounds.size.y});
    // add verts to the vertex array
    sf::Color colour = sprite.getColor();
    vertices.emplace_back(sf::Vertex{topLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y)}});
    vertices.emplace_back(sf::Vertex{topRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}});
    vertices.emplace_back(sf::Vertex{bottomLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    
    vertices.emplace_back(sf::Vertex{bottomLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    vertices.emplace_back(sf::Vertex{bottomRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    vertices.emplace_back(sf::Vertex{topRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}});
}

void BatchRenderer::AddRectangleToBatch(const sf::RectangleShape& rectShape, std::vector<sf::Vertex>& rectTriangles) {
    sf::FloatRect bounds = rectShape.getGlobalBounds();
    sf::Vector2f topLeft{bounds.position.x,bounds.position.y};
    sf::Vector2f topRight{bounds.position.x+bounds.size.x, bounds.position.y};
    sf::Vector2f bottomLeft{bounds.position.x, bounds.position.y+bounds.size.y};
    sf::Vector2f bottomRight{bounds.position.x+bounds.size.x,bounds.position.y+bounds.size.y};
    sf::Color colour = rectShape.getFillColor();
    rectTriangles.push_back(sf::Vertex({topLeft, colour}));
    rectTriangles.push_back(sf::Vertex({topRight, colour}));
    rectTriangles.push_back(sf::Vertex({bottomLeft, colour}));

    rectTriangles.push_back(sf::Vertex({bottomLeft, colour}));
    rectTriangles.push_back(sf::Vertex({bottomRight, colour}));
    rectTriangles.push_back(sf::Vertex({topRight, colour}));
}


constexpr float CIRCLE_SEGMENTS = 50.f; // 50 orig
void BatchRenderer::SetFlameTriangles(std::vector<Flame>& flames, float initalRadius) {
    flameTriangles.clear();
    std::vector<sf::Vector2f> unitCircle;
    for (int i = 0; i <= CIRCLE_SEGMENTS; ++i) {
        // get radians of circle split evenly
        float angleInRadians = (i / CIRCLE_SEGMENTS) * 2 * M_PI; //2PI represent whole circle
        // unit circle so radius(hypotenuse) is 1, cos angle is x coord, sin is y coord
        unitCircle.push_back({std::cos(angleInRadians), std::sin(angleInRadians)});
    }
    for (const Flame& flame : flames) {
        sf::Vector2f center = flame.position;
        sf::Color colour = flame.colour;
        float radius = 10.f * flame.size;
        // create a circle made up of triangles, all have same center vertex, with the outer verts
        // made by scaling the unit circle with the given radius
        for (int i = 0; i < CIRCLE_SEGMENTS; ++i) {
            sf::Vector2f p1 = center + unitCircle[i] * radius;
            sf::Vector2f p2 = center + unitCircle[i+1] * radius;
            
            flameTriangles.append(sf::Vertex({center, colour}));    
            flameTriangles.append(sf::Vertex({p1, colour}));
            flameTriangles.append(sf::Vertex({p2, colour}));
        }
    }
}

void BatchRenderer::RenderFlameTriangles(){
    if(flameTriangles.getVertexCount() > 0){
        window.draw(flameTriangles, sf::RenderStates(sf::BlendAdd));
        flameTriangles.clear();
    }
}

void BatchRenderer::AddHpBarTriangles(std::reference_wrapper<Character>& character) {
    AddRectangleToBatch(character.get().hud.hpBarBackground, hpBarTriangles);
    AddRectangleToBatch(character.get().hud.damageBar, hpBarTriangles);
    AddRectangleToBatch(character.get().hud.hpBar, hpBarTriangles);
    for(auto& line : character.get().hud.segmentLines) {
        AddRectangleToBatch(line, hpBarTriangles);
    }
}

void BatchRenderer::DrawBufferedMonsterTriangles() { // #TODO WILL NEED TO HANDLE BUFFER, CLEARING, TEXTURE SWITCHES WITH MONSTERS BETTER
    static std::map<MonsterE, sf::Texture*> monsterTextureMap { // LOOP THROUGH MONSTER TEXTURE MAP, DRAW WHATEVER MONSTERS IS NOT NULL?
        {MonsterE::LOW, AnimUtil::lowMonsterTexture}, // WHILE KEEPING TRACK OF CURRENT MONSTER TEXTURE?
        {MonsterE::WOLF, AnimUtil::wolfTexture}, // #TODO FIX Y ORDER FOR MONSTERS
    };

    for (auto it = monsterTriangles.begin(); it != monsterTriangles.end(); ++it) {
        window.draw((it->second).data(), (it->second).size(), sf::PrimitiveType::Triangles, monsterTextureMap[it->first]);
    }
}

void BatchRenderer::ClearMonsterTriangles(){
    for (auto it = monsterTriangles.begin(); it != monsterTriangles.end(); ++it) {
        it->second.clear();
    }
}

void BatchRenderer::BatchRenderCharacters(std::vector<std::reference_wrapper<Character>>& characters){
    // #TODO FIX Y ORDER FOR MONSTERS
    std::sort(characters.begin(), characters.end(),
    [](const std::reference_wrapper<Character>& firstCharacter, const std::reference_wrapper<Character>& secondCharacter) {
        if (firstCharacter.get().IsDead() && !secondCharacter.get().IsDead()) {
            return true;
        }
        if (!firstCharacter.get().IsDead() && secondCharacter.get().IsDead()) {
            return false;
        }
        return firstCharacter.get().GetYOrdering() < secondCharacter.get().GetYOrdering();
    });
     // clear previous vertices
    ClearMonsterTriangles();
    hpBarTriangles.clear();
     // add each sprites vertices to vertex array
    for(auto& character : characters){
        if(auto zombie = dynamic_cast<Zombie*>(&character.get())) {
            AddSpriteToBatch(zombie->sprite, monsterTriangles[MonsterE::LOW]);
        } else if(auto smallDemon = dynamic_cast<SmallDemon*>(&character.get())) {
            AddSpriteToBatch(smallDemon->sprite, monsterTriangles[MonsterE::LOW]);
        } else if(auto bigDemon = dynamic_cast<BigDemon*>(&character.get())) {
            AddSpriteToBatch(bigDemon->sprite, monsterTriangles[MonsterE::LOW]);
        } else if(auto wolf = dynamic_cast<Wolf*>(&character.get())) {
            AddSpriteToBatch(wolf->sprite, monsterTriangles[MonsterE::WOLF]);
        } else if(auto player = dynamic_cast<Player*>(&character.get())) {
            // if it's a player, draw all of the monsters prior, draw player on top, clear buffer
            DrawBufferedMonsterTriangles();
            if(!character.get().IsDead()){
                AddHpBarTriangles(character);
            }
            ClearMonsterTriangles();
            player->Draw(window, *this);
            continue;
        }
        if(!character.get().IsDead()){
            AddHpBarTriangles(character);
        }   
    }
    DrawBufferedMonsterTriangles();
    window.draw(hpBarTriangles.data(), hpBarTriangles.size(),  sf::PrimitiveType::Triangles, sf::RenderStates::Default);
}