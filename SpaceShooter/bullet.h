#pragma once
#include<SFML/Graphics.hpp>
using namespace sf;
class Bullet {
public:
	CircleShape shape;
	float speed = 20.f;
	Bullet(Vector2f startPos) {
		shape.setRadius(5.f);
		shape.setFillColor(Color::Red);
		shape.setPosition(startPos);
	}
	void update() {
		shape.move({0.f, -speed});
		}
	bool isOffScreen() const {
		return shape.getPosition().y + shape.getRadius() < 0;
	}
};