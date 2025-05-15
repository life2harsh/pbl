#pragma once
#include<SFML/Graphics.hpp>
#include<vector>
#include "bullet.h"
using namespace std;
using namespace sf;

class Player {
private:
	Texture texture;
	Sprite sprite{ texture };
	float speed = 5.f;

public:
	vector<Bullet> bullets;
	Player() = default;
	explicit Player(const string& path)
	{
		if (!load(path))
			throw runtime_error("Failed to load player texture: " + path);
	}

	bool load(const string& path) {
		if (!texture.loadFromFile(path)) return false;
		sprite.setTexture(texture, /* resetRect= */ true);
		sprite.setPosition({ 800.f, 600.f });
		sprite.setScale({ 0.1f, 0.1f });
		return true;
	}

	void handleInput() {
		Vector2f movement(0.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::Key::Left)|| Keyboard::isKeyPressed(Keyboard::Key::A)) movement.x -= speed;
		if (Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D)) movement.x += speed;
		if (Keyboard::isKeyPressed(Keyboard::Key::Up) || Keyboard::isKeyPressed(Keyboard::Key::W)) movement.y -= speed;
		if (Keyboard::isKeyPressed(Keyboard::Key::Down) || Keyboard::isKeyPressed(Keyboard::Key::S)) movement.y += speed;
		sprite.move(movement);
	}

	void shoot() {
		sf::FloatRect bounds = sprite.getGlobalBounds();
		Vector2f bulletStart;
		bulletStart.x = bounds.position.x + bounds.size.x / 2.f - 2.5f;
		bulletStart.y = bounds.position.y;

		bullets.emplace_back(bulletStart);
	}

	void updateBullet() {
		for (auto& b : bullets) b.update();
		bullets.erase(
			remove_if(bullets.begin(), bullets.end(),
				[](const Bullet& b)
				{
					return b.isOffScreen();
				}),
			bullets.end()
		);
	}

	FloatRect getBounds() const {
		return sprite.getGlobalBounds();
	}

	void draw(RenderWindow& window) {
		window.draw(sprite);
		for (auto& b : bullets) window.draw(b.shape);
	}
};