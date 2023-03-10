#pragma once

#include <iostream>
#include <Graphics.hpp>
#include <vector>
#include <Window.hpp>
#include <cstdlib>
#include <Audio.hpp>

class Object : public sf::RectangleShape
{
public:

	Object() : sf::RectangleShape(), m_velocity(sf::Vector2f(0.0f, 0.0f)) {}
	void spriteFollow(Object& object, float x, float y);
	void setVelocity(Object& object, sf::Vector2f newVelocity);
	void checkCollision(std::vector<Object>& objects1, std::vector<Object>& objects2, Object& g_objectClass, float gravityChange, float velocityChangeX, float velocityChangeY, bool& objectCollides);
	virtual void applyGravity(Object& object, float gravity, float velocityLimit);
	virtual void applyHorizontalMovement(Object& object, float x, float velocityLimit);
	virtual void applyTexture(Object& object, const std::string& texturePath);
	virtual void setPosition(Object& object, float x, float y);
	virtual void createObject(int width, int length, int xstart, int ystart, int r, int g, int b, int type, bool isTransparent);
	virtual void deleteObject(sf::Vector2f mousePos, int type);
	bool isColliding(const Object& other) const;
	sf::RectangleShape m_shape;
	sf::Vector2f m_velocity;
	sf::Sprite m_sprite;
	sf::RectangleShape getShape() const;

};

class Camera
{
public:

	Camera() : m_position(0.0f, 0.0f), m_movement(0.0f, 0.0f) {}
	void update(sf::Time deltaTime);
	void setMovement(sf::Vector2f movement);
	void setPosition(sf::Vector2f position);
	void viewFollow(const sf::Vector2f& objectPos, sf::RenderWindow& window);
	sf::Vector2f getMovement() const;
	sf::Vector2f getPosition() const;
	
private:

	sf::Vector2f m_position;
	sf::Vector2f m_movement;

};

class TextObject
{
private:

	sf::Text m_text;
	sf::Font m_font;

public:

	TextObject(std::string text, int x, int y, int size, int r, int g, int b)
	{
		m_font.loadFromFile("font.ttf");
		m_text.setString(text);
		m_text.setPosition(x, y);
		m_text.setCharacterSize(size);
		m_text.setFillColor(sf::Color(r, g, b));
		m_text.setFont(m_font);
	}

	void setText(std::string text);
	void setPosition(int x, int y);
	void setColor(int r, int g, int b);
	void setSize(int size);
	sf::Text getText();

};
