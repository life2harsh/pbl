#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
private:
    sf::Sprite   sprite;
    float        speed;

public:
    Enemy(const sf::Texture& tex, sf::Vector2f startPos, float speed)
        : sprite(tex)
        , speed(speed)
    {
        auto lb = sprite.getLocalBounds();
        sprite.setOrigin({ lb.size.x / 2.f, lb.size.y / 2.f }); // Corrected: Added {} for Vector2f

        sprite.setScale({ 0.05f, 0.05f });

        sprite.setPosition(startPos);
    }

    void update(float dt) {
        sprite.move({ 0.08f, speed * dt });
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    bool isOffScreen(unsigned windowHeight) const {
        auto b = getBounds();
        return b.position.y - (b.size.y / 2.f) > static_cast<float>(windowHeight);
    }

    void setScale(const sf::Vector2f& scale) {
        sprite.setScale(scale);
    }
};