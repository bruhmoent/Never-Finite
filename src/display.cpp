#include <Graphics.hpp>
#include <Window.hpp>
#include <iostream>
#include <windows.h>
#include <iostream>
#include "object.h"

Object g_objectClass;
bool g_playerObjectCollides = false;
bool g_moveLeft = false;
bool g_moveRight = false;
bool g_playerObjectJump = false;

int main() {

	sf::Event event;
	extern std::vector<Object> g_ColliderArray;
	extern std::vector<Object> g_NonColliderArray;
	extern std::vector<Object> g_PlayerArray;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Never Finite");
	g_objectClass.createObject(64, 64, 0, 0, 255, 0, 0, 2);
	g_objectClass.createObject(400, 64, 0, 400, 200, 0, 55, 0);
	
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2f l_mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				g_objectClass.createObject(64, 64, l_mousePos.x, l_mousePos.y, 0, 255, 0, 0);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				sf::Vector2f l_mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				g_objectClass.deleteObject(l_mousePos, 0);
			}
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					g_moveLeft = true;
					break;
				case sf::Keyboard::Right:
					g_moveRight = true;
					break;
				default:
					break;
				}
			}
			else if (event.type == sf::Event::KeyReleased)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					g_moveLeft = false;
					break;
				case sf::Keyboard::Right:
					g_moveRight = false;
					break;
				case sf::Keyboard::Up:
					for (int i = 0; i < g_PlayerArray.size(); i++) {
						g_playerObjectJump = false;
					}
					break;
				default:
					break;
				}
			}
		}

		window.clear();
		if (!g_playerObjectCollides) {
			for (int i = 0; i < g_PlayerArray.size(); i++) { g_objectClass.applyGravity(g_PlayerArray[i], 0.5f, 1.0f); }
			g_playerObjectCollides = false;
		}

		for (int i = 0; i < g_PlayerArray.size(); i++) {
			if (g_moveLeft) g_objectClass.applyVerticalMovement(g_PlayerArray[i], -0.5f, -1.0f);
			if (g_moveRight) g_objectClass.applyVerticalMovement(g_PlayerArray[i], 0.5f, 1.0f);
		}

		g_playerObjectCollides = false;

		for (int i = 0; i < g_PlayerArray.size(); i++) {
			for (int j = 0; j < g_ColliderArray.size(); j++) {
				if (g_PlayerArray[i].getShape().getGlobalBounds().intersects(g_ColliderArray[j].getShape().getGlobalBounds())) {
					float l_playerBottom = g_PlayerArray[i].getShape().getPosition().y + g_PlayerArray[i].getShape().getSize().y;
					float l_floorTop = g_ColliderArray[j].getShape().getPosition().y;
					float l_floorHeight = g_ColliderArray[j].getShape().getSize().y;
					float l_playerTop = g_PlayerArray[i].getShape().getPosition().y;
					float l_playerLeft = g_PlayerArray[i].getShape().getPosition().x;
					float l_playerRight = g_PlayerArray[i].getShape().getPosition().x + g_PlayerArray[i].getShape().getSize().x;
					float l_wallLeft = g_ColliderArray[j].getShape().getPosition().x;
					float l_wallRight = g_ColliderArray[j].getShape().getPosition().x + g_ColliderArray[j].getShape().getSize().x;
					float l_blockHeight = g_ColliderArray[j].getShape().getPosition().y + g_ColliderArray[j].getShape().getSize().y;
					if (l_playerLeft < l_wallRight && l_playerRight > l_wallLeft) {
						if (g_PlayerArray[i].getShape().getPosition().x + g_PlayerArray[i].getShape().getSize().x / 2 < g_ColliderArray[j].getShape().getPosition().x) {
							float direction = g_PlayerArray[i].getShape().getPosition().x - (g_ColliderArray[j].getShape().getPosition().x - g_PlayerArray[i].getShape().getSize().x);
							if (direction > 0) {
								g_objectClass.applyVerticalMovement(g_PlayerArray[i], 0.5f, 1.0f);
							}
							else {
								g_objectClass.applyVerticalMovement(g_PlayerArray[i], -0.5f, -1.0f);
							}
							if (l_blockHeight > l_playerTop + (g_PlayerArray[i].getShape().getSize().y / 3)) {
								g_objectClass.applyGravity(g_PlayerArray[i], -0.7f, -1.2f);
								g_PlayerArray[i].getShape().setPosition(g_ColliderArray[j].getShape().getPosition().x - g_PlayerArray[i].getShape().getSize().x, g_PlayerArray[i].getShape().getPosition().y);
							}
							g_PlayerArray[i].getShape().setPosition(g_ColliderArray[j].getShape().getPosition().x - g_PlayerArray[i].getShape().getSize().x, g_PlayerArray[i].getShape().getPosition().y);
						}
						else {
							g_PlayerArray[i].getShape().setPosition(g_ColliderArray[j].getShape().getPosition().x + g_ColliderArray[j].getShape().getSize().x, g_PlayerArray[i].getShape().getPosition().y);
						}
						g_objectClass.setVelocity(g_PlayerArray[i], sf::Vector2f(0.0f, 0.0f));
					}
					if (l_playerBottom > l_floorTop) {
						g_playerObjectCollides = true;
						g_PlayerArray[i].getShape().setPosition(g_PlayerArray[i].getShape().getPosition().x, g_ColliderArray[j].getShape().getPosition().y - g_PlayerArray[i].getShape().getSize().y);
						g_objectClass.setVelocity(g_PlayerArray[i], sf::Vector2f(0.0f, 0.0f));
					}
					else { g_playerObjectCollides = false; }
				}
			}
		}

		for (const auto& collider : g_ColliderArray){window.draw(collider.getShape());}

		for (const auto& nonCollider : g_NonColliderArray){window.draw(nonCollider.getShape());}

		for (const auto& player : g_PlayerArray){window.draw(player.getShape());}

		window.display();
	}
	return 0;
}
