#pragma once
#include <iostream>
#include <Graphics.hpp>
#include <vector>
class Object : public sf::RectangleShape
{
public:
	Object() : sf::RectangleShape(), m_velocity(sf::Vector2f(0.0f, 0.0f)) {}
	sf::RectangleShape m_shape;
	sf::Vector2f m_velocity;
	void setVelocity(Object& object, sf::Vector2f newVelocity);
	sf::RectangleShape getShape() const;
	virtual void createObject(int width, int length, int xstart, int ystart, int r, int g, int b, int type);
	virtual void applyGravity(Object& object, float gravity, float velocityLimit);
	virtual void applyVerticalMovement(Object& object, float x, float velocityLimit);
};
