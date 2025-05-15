#pragma once
#include<SFML/Graphics.hpp>
#include<cmath>
using namespace std;
using namespace sf;

class Asteroid {
private:
	Sprite sprite;
	float speed;

public:
	Asteroid(const Texture& tex, Vector2f startpos, float speed) : sprite(tex), speed(speed) {
		sprite.setScale({ 0.3f, 0.3f });
		//sprite.setColor(sf::Color::Red);
		sprite.setPosition(startpos);
	}

	void update(float dt) {
		float wobble = std::sin(sprite.getPosition().y * 0.05f) * 10.f * dt;
		sprite.move({ wobble, speed * dt });
	}

	void draw(RenderWindow& window) {
		window.draw(sprite);
		/*sf::CircleShape debug(10.f);
		debug.setFillColor(sf::Color::Cyan);
		debug.setPosition(sprite.getPosition());
		window.draw(debug);*/
	}

	FloatRect getBounds() const {
		return sprite.getGlobalBounds();
	}

	bool isOffScreen(unsigned windowheight) const
	{
		auto bounds = getBounds();
		return sprite.getPosition().y - (bounds.size.y / 2.f) > windowheight;
	}

};