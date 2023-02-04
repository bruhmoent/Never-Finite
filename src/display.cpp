#include <Graphics.hpp>
#include <Window.hpp>
#include <iostream>
#include <windows.h>
#include <iostream>
#include "object.h"
#include <cstdlib>

const int g_spawnRadius = 1000;
const int g_coinLimit = 10;
int g_levelCount = 1;
int g_coinCount = 0;
Object g_objectClass;
bool g_playerObjectCollides = false;
bool g_moveLeft = false;
bool g_moveRight = false;
bool g_playerObjectJump = false;
Camera g_camera;
TextObject g_text("Coins: " + std::to_string(g_coinCount), 100, 100, 32, 255, 255, 255);
TextObject g_levelText("Level: " + std::to_string(g_levelCount), 100, 100, 32, 255, 255, 255);

void createCoins(int radius, int amount)
{
	for (int i = 0; i < amount; i++)
	{
		int x = rand() % radius + 100;
		int y = rand() % radius + 100;
		g_objectClass.createObject(64, 64, x, y, 250, 222, 40, 3, false);
	}
}

int main() {

	sf::Event event;
	extern std::vector<Object> g_ColliderArray;
	extern std::vector<Object> g_NonColliderArray;
	extern std::vector<Object> g_PlayerArray;
	extern std::vector<Object> g_InterActiveObjectArray;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Never Finite");
	window.setFramerateLimit(60);
	g_objectClass.createObject(64, 64, 0, 0, 255, 0, 0, 2, true);
	g_objectClass.createObject(400, 64, 0, 400, 118, 68, 166, 0, false);
	g_objectClass.applyTexture(g_PlayerArray[0], "images/player.png");
	createCoins(g_spawnRadius, g_coinLimit);

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
				g_objectClass.createObject(64, 64, l_mousePos.x, l_mousePos.y, 89, 10, 166, 0, false);
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

		window.clear(sf::Color(48, 25, 52));

		if (!g_playerObjectCollides) {
			for (int i = 0; i < g_PlayerArray.size(); i++) { g_objectClass.applyGravity(g_PlayerArray[i], 5.0f, 5.0f); }
			g_playerObjectCollides = false;
		}

		for (int i = 0; i < g_PlayerArray.size(); i++) {
			g_text.setPosition(g_PlayerArray[i].getShape().getPosition().x + 250, g_PlayerArray[i].getShape().getPosition().y-300);
			g_levelText.setPosition(g_PlayerArray[i].getShape().getPosition().x - 350, g_PlayerArray[i].getShape().getPosition().y - 300);
			if (g_moveLeft) g_objectClass.applyVerticalMovement(g_PlayerArray[i], -4.5f, -5.0f);
			if (g_moveRight) g_objectClass.applyVerticalMovement(g_PlayerArray[i], 4.5f, 5.0f);
		}

		g_playerObjectCollides = false;

		for (int i = 0; i < g_PlayerArray.size(); i++){ g_camera.followObject(g_PlayerArray[i].m_shape.getPosition(), window); }

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
								g_objectClass.applyGravity(g_PlayerArray[i], 3.0f, 3.0f);
							}
							else {
								g_objectClass.applyVerticalMovement(g_PlayerArray[i], -2.5f, -3.0f);
							}
							if (l_blockHeight > l_playerTop + (g_PlayerArray[i].getShape().getSize().y / 3)) {
								g_objectClass.applyGravity(g_PlayerArray[i], -5.0f, -5.2f);
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

		for (int i = 0; i < g_PlayerArray.size(); i++) {
			for (int j = 0; j < g_InterActiveObjectArray.size(); j++) {
				if (g_PlayerArray[i].isColliding(g_InterActiveObjectArray[j])) {
					g_coinCount++;
					g_text.setText("Coins: " + std::to_string(g_coinCount));
					g_InterActiveObjectArray.erase(g_InterActiveObjectArray.begin() + j);
					break;
				}
			}
		}

		for (const auto& collider : g_ColliderArray){ window.draw(collider.getShape());}

		for (const auto& nonCollider : g_NonColliderArray){window.draw(nonCollider.getShape());}

		for (const auto& player : g_PlayerArray) { window.draw(player.getShape()); }

		for (const auto& interactiveObject : g_InterActiveObjectArray){window.draw(interactiveObject.getShape());}

		window.draw(g_text.getText());
		window.draw(g_levelText.getText());

		if (g_coinCount == g_coinLimit)
		{
			g_coinCount = 0;
			g_ColliderArray.clear();
			g_levelCount++;
			createCoins(g_spawnRadius, g_coinLimit);
			g_levelText.setText("Level: " + std::to_string(g_levelCount));
			g_text.setText("Coins: " + std::to_string(g_coinCount));
			for (int i = 0; i < g_PlayerArray.size(); i++) {
				g_objectClass.setPosition(g_PlayerArray[i], 50.0f, 300.0f);
				g_objectClass.setVelocity(g_PlayerArray[i], sf::Vector2f(0.0f, 0.0f));
			}
			g_camera.setPosition(sf::Vector2f(0, 0));
			g_objectClass.createObject(400, 64, 0, 400, 118, 68, 166, 0, false);
		}
		for (int i = 0; i < g_PlayerArray.size(); i++)
		{
			g_PlayerArray[i].m_sprite.setPosition(g_PlayerArray[i].getShape().getPosition().x, g_PlayerArray[i].getShape().getPosition().y);
			window.draw(g_PlayerArray[i].m_sprite);
		}
		window.display();
	}
	return 0;
}
