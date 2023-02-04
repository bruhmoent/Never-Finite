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
	virtual void deleteObject(sf::Vector2f mousePos, int type);
	virtual void applyGravity(Object& object, float gravity, float velocityLimit);
	virtual void applyVerticalMovement(Object& object, float x, float velocityLimit);
};

class Camera
{
public:
    Camera() : m_position(0.0f, 0.0f), m_movement(0.0f, 0.0f) {}
	void update(sf::Time deltaTime);
	void setMovement(sf::Vector2f movement);
	void setPosition(sf::Vector2f position);
	sf::Vector2f getMovement() const;
	sf::Vector2f getPosition() const;
private:
    sf::Vector2f m_position;
    sf::Vector2f m_movement;
};
