#include "object.h"
std::vector<Object> g_ColliderArray;
std::vector<Object> g_NonColliderArray;
std::vector<Object> g_PlayerArray;
void Object::setVelocity(Object& object, sf::Vector2f newVelocity)
{
	object.m_velocity = newVelocity;
}

void Object::createObject(int width, int length, int xstart, int ystart, int r, int g, int b, int type)
{
    m_shape = sf::RectangleShape(sf::Vector2f(width, length));
    m_shape.setPosition(sf::Vector2f(xstart, ystart));
    m_shape.setFillColor(sf::Color(r, g, b));
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
    }
}
sf::RectangleShape Object::getShape() const
{
    return m_shape;
}
	void Object::applyGravity(Object& object, float gravity, float velocityLimit)
	{
        object.m_velocity.y += gravity;
        if (object.m_velocity.y > velocityLimit)
            object.m_velocity.y = velocityLimit;
        object.m_shape.move(sf::Vector2f(0, object.m_velocity.y));
	}

    void Object::applyVerticalMovement(Object& object, float x, float velocityLimit)
    {
        object.m_velocity.x += x;
        if (object.m_velocity.x > velocityLimit)
            object.m_velocity.x = velocityLimit;
        object.m_shape.move(sf::Vector2f(object.m_velocity.x, object.m_velocity.y));
    }
