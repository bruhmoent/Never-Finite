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

    void Camera::update(sf::Time deltaTime) { m_position += m_movement * deltaTime.asSeconds(); }

    void Camera::setMovement(sf::Vector2f movement) { m_movement = movement; }

    void Camera::setPosition(sf::Vector2f position) { m_position = position; }

    sf::Vector2f Camera::getMovement() const { return m_movement; }

    sf::Vector2f Camera::getPosition() const { return m_position; }
