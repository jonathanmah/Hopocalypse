#include "entities/effects/Knockback.h"
#include "entities/Character.h"

Knockback::Knockback(Character& character) : character(character), knockbackDebt(0.f), knockbackVector({0,0}){}

void Knockback::SetKnockback(float newKnockbackDistance, sf::Vector2f newKnockbackVector) {
    knockbackVector = newKnockbackVector;
    knockbackDebt = newKnockbackDistance;
}

void Knockback::ApplyKnockback() {
    character.GetSprite().move(knockbackVector);
    knockbackDebt = std::max(0.f, knockbackDebt - knockbackVector.length());
}