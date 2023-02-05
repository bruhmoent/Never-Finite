#include "object.h"

std::vector<Object> g_ColliderArray;
std::vector<Object> g_NonColliderArray;
std::vector<Object> g_PlayerArray;
std::vector<Object> g_InterActiveObjectArray;

void Object::setVelocity(Object& object, sf::Vector2f newVelocity)
{
	object.m_velocity = newVelocity;
}

void Object::createObject(int width, int length, int xstart, int ystart, int r, int g, int b, int type, bool isTransparent)
{
	m_shape = sf::RectangleShape(sf::Vector2f(width, length));
	m_shape.setPosition(sf::Vector2f(xstart, ystart));
	if (isTransparent)
	{
		m_shape.setFillColor(sf::Color(r, g, b, 0));
	}
	else
	{
		m_shape.setFillColor(sf::Color(r, g, b));
	}
	switch (type)
	{
	case 0:
		g_ColliderArray.push_back(*this);
		break;
	case 1:
		g_NonColliderArray.push_back(*this);
		break;
	case 2:
		g_PlayerArray.push_back(*this);
		break;
	case 3:
		g_InterActiveObjectArray.push_back(*this);
		break;
	}
}

void Object::deleteObject(sf::Vector2f mousePos, int type)
{
	sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	switch (type)
	{
	case 0:
		for (int i = 0; i < g_ColliderArray.size(); i++)
		{
			if (g_ColliderArray[i].getShape().getGlobalBounds().contains(mousePosF))
			{
				g_ColliderArray.erase(g_ColliderArray.begin() + i);
				break;
			}
		}
		break;
	case 1:
		for (int i = 0; i < g_NonColliderArray.size(); i++)
		{
			if (g_NonColliderArray[i].getShape().getGlobalBounds().contains(mousePosF))
			{
				g_NonColliderArray.erase(g_NonColliderArray.begin() + i);
				break;
			}
		}
		break;
	case 2:
		for (int i = 0; i < g_PlayerArray.size(); i++)
		{
			if (g_PlayerArray[i].getShape().getGlobalBounds().contains(mousePosF))
			{
				g_PlayerArray.erase(g_PlayerArray.begin() + i);
				break;
			}
		}
		break;
	case 3:
		for (int i = 0; i < g_InterActiveObjectArray.size(); i++)
		{
			if (g_InterActiveObjectArray[i].getShape().getGlobalBounds().contains(mousePosF))
			{
				g_InterActiveObjectArray.erase(g_InterActiveObjectArray.begin() + i);
				break;
			}
		}
		break;
	}
}

sf::RectangleShape Object::getShape() const
{
	return m_shape;
}

bool Object::isColliding(const Object& other) const {
	return m_shape.getGlobalBounds().intersects(other.m_shape.getGlobalBounds());
}

void Object::applyGravity(Object& object, float gravity, float velocityLimit)
{
	object.m_velocity.y += gravity;
	if (object.m_velocity.y > velocityLimit)
		object.m_velocity.y = velocityLimit;
	object.m_shape.move(sf::Vector2f(0, object.m_velocity.y));
}

void Object::applyHorizontalMovement(Object& object, float x, float velocityLimit)
{
	object.m_velocity.x += x;
	if (object.m_velocity.x > velocityLimit)
		object.m_velocity.x = velocityLimit;
	object.m_shape.move(sf::Vector2f(object.m_velocity.x, object.m_velocity.y));
}

void Camera::update(sf::Time deltaTime) { m_position += m_movement * deltaTime.asSeconds(); }

void Camera::setMovement(sf::Vector2f movement) { m_movement = movement; }

void Camera::setPosition(sf::Vector2f position) { m_position = position; }

sf::Vector2f Camera::getMovement() const { return m_movement; }

sf::Vector2f Camera::getPosition() const { return m_position; }

void Camera::viewFollow(const sf::Vector2f& objectPos, sf::RenderWindow& window)
{
	sf::View view = window.getView();
	view.setCenter(objectPos);
	window.setView(view);
}

void TextObject::setText(std::string text)
{
	m_text.setString(text);
}

void TextObject::setPosition(int x, int y)
{
	m_text.setPosition(x, y);
}

void TextObject::setColor(int r, int g, int b)
{
	m_text.setFillColor(sf::Color(r, g, b));
}

void TextObject::setSize(int size)
{
	m_text.setCharacterSize(size);
}

sf::Text TextObject::getText()
{
	return m_text;
}

void Object::setPosition(Object& object, float x, float y)
{
	object.m_shape.setPosition(x, y);
	object.m_velocity = sf::Vector2f(0.0f, 0.0f);
}
void Object::spriteFollow(Object& object, float x, float y)
{
	object.m_sprite.setPosition(x, y);
}
void Object::applyTexture(Object& object, const std::string& texturePath)
{
	sf::Texture* l_texture = new sf::Texture;
	l_texture->setSmooth(true);
	l_texture->setRepeated(true);
	l_texture->loadFromFile(texturePath);
	if (!l_texture->loadFromFile(texturePath))
		return;
	object.m_sprite.setTexture(*l_texture);
	object.m_sprite.setPosition(object.m_shape.getPosition().x, object.m_shape.getPosition().y);
	object.m_sprite.setTextureRect(sf::IntRect(0, 0, object.m_shape.getSize().x, object.m_shape.getSize().y));
}
void Object::checkCollision(std::vector<Object>& objects1, std::vector<Object>& objects2, Object& objectClass, float gravityChange, float velocityChangeX, float velocityChangeY, bool& objectCollides)
{
	for (int i = 0; i < objects1.size(); i++) {
		for (int j = 0; j < objects2.size(); j++) {
			if (objects1[i].getShape().getGlobalBounds().intersects(objects2[j].getShape().getGlobalBounds())) {
				bool onGround = false;
				float l_playerBottom = objects1[i].getShape().getPosition().y + objects1[i].getShape().getSize().y;
				float l_floorTop = objects2[j].getShape().getPosition().y;
				float l_floorHeight = objects2[j].getShape().getSize().y;
				float l_playerTop = objects1[i].getShape().getPosition().y;
				float l_playerLeft = objects1[i].getShape().getPosition().x;
				float l_playerRight = objects1[i].getShape().getPosition().x + objects1[i].getShape().getSize().x;
				float l_wallLeft = objects2[j].getShape().getPosition().x;
				float l_wallRight = objects2[j].getShape().getPosition().x + objects2[j].getShape().getSize().x;
				float l_blockHeight = objects2[j].getShape().getPosition().y + objects2[j].getShape().getSize().y;
				if (l_playerLeft < l_wallRight && l_playerRight > l_wallLeft) {
					if (objects1[i].getShape().getPosition().x + objects1[i].getShape().getSize().x / 2 < objects2[j].getShape().getPosition().x) {
						float direction = objects1[i].getShape().getPosition().x - (objects2[j].getShape().getPosition().x - objects1[i].getShape().getSize().x);
						if (direction > 0) {
							objectClass.applyGravity(objects1[i], gravityChange, gravityChange + 0.2f);
						}
						else {
							objectClass.applyHorizontalMovement(objects1[i], -velocityChangeX, -velocityChangeX + 0.2f);
						}

						if (l_blockHeight > l_playerTop + (objects1[i].getShape().getSize().y / 3)) {
								objectClass.applyGravity(objects1[i], -gravityChange * 2, -gravityChange * 2 + 0.2f);
								objects1[i].getShape().setPosition(objects2[j].getShape().getPosition().x - objects1[i].getShape().getSize().x, objects1[i].getShape().getPosition().y);
						}
						
						objects1[i].getShape().setPosition(objects2[j].getShape().getPosition().x - objects1[i].getShape().getSize().x, objects1[i].getShape().getPosition().y);
					}
					else {
						objects1[i].getShape().setPosition(objects2[j].getShape().getPosition().x + objects2[j].getShape().getSize().x, objects1[i].getShape().getPosition().y);
					}
					objectClass.setVelocity(objects1[i], sf::Vector2f(0.0f, 0.0f));
				}
				if (l_playerBottom > l_floorTop) {
					objectCollides = true;
					objects1[i].getShape().setPosition(objects1[i].getShape().getPosition().x, objects2[j].getShape().getPosition().y - objects1[i].getShape().getSize().y);
					objectClass.setVelocity(objects1[i], sf::Vector2f(0.0f, 0.0f));
				}
				else { objectCollides = false; }
			}
		}
	}
}
