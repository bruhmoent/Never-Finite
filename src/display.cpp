#include "object.h"

const int g_spawnRadius = 1000;
const int g_coinLimit = 10;
int g_levelCount = 1;
int g_coinCount = 0;
bool g_playerObjectCollides = false;
bool g_moveLeft = false;
bool g_moveRight = false;
bool g_playerObjectJump = false;
extern std::vector<Object> g_ColliderArray;
extern std::vector<Object> g_NonColliderArray;
extern std::vector<Object> g_PlayerArray;
extern std::vector<Object> g_InterActiveObjectArray;
sf::Event g_event;
sf::RenderWindow g_window(sf::VideoMode(800, 600), "Never Finite");
sf::Music g_music;
Object g_objectClass;
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

void updateDisplayedCoins(std::vector<Object>& objectArray, std::vector<Object>& interactiveArray, int& coinCount, TextObject &coinText) {
	for (int i = 0; i < objectArray.size(); i++) {
		for (int j = 0; j < interactiveArray.size(); j++) {
			if (objectArray[i].isColliding(interactiveArray[j])) {
				coinCount++;
				coinText.setText("Coins: " + std::to_string(coinCount));
				interactiveArray.erase(interactiveArray.begin() + j);
				break;
			}
		}
	}
}

void resetGameLoop(int& coinCount, Camera camera, const int coinLimit, const int spawnRadius, int& levelCount, std::vector<Object>& objectArray, std::vector<Object>& colliderArray, TextObject &levelText, TextObject &coinText, float resetPosX, float resetPosY, Object objectClass)
{
	if (coinCount == coinLimit)
	{
		coinCount = 0;
		colliderArray.clear();
		levelCount++;
		createCoins(spawnRadius, coinLimit);
		levelText.setText("Level: " + std::to_string(g_levelCount));
		coinText.setText("Coins: " + std::to_string(g_coinCount));
		for (int i = 0; i < objectArray.size(); i++) {
			objectClass.setPosition(objectArray[i], resetPosX, resetPosY);
			objectClass.setVelocity(objectArray[i], sf::Vector2f(0.0f, 0.0f));
		}
		camera.setPosition(sf::Vector2f(0, 0));
		objectClass.createObject(400, 64, 0, 400, 118, 68, 166, 0, false);
	}
}

void handleKeyPressMain(const sf::Event& event) {
	switch (event.key.code) {
	case sf::Keyboard::Left:
		g_moveLeft = true;
		break;
	case sf::Keyboard::Right:
		g_moveRight = true;
		break;
	case sf::Keyboard::X:
		for (int i = 0; i < g_PlayerArray.size(); i++) {
			g_objectClass.setPosition(g_PlayerArray[i], 50.0f, 300.0f);
			g_objectClass.setVelocity(g_PlayerArray[i], sf::Vector2f(0.0f, 0.0f));
		}
		break;
	default:
		break;
	}
}

void handleKeyReleaseMain(const sf::Event& event) {
	switch (event.key.code) {
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

void handleMousePressMain() {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2f mousePos = g_window.mapPixelToCoords(sf::Mouse::getPosition(g_window));
			g_objectClass.createObject(64, 64, mousePos.x, mousePos.y, 89, 10, 166, 0, false);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vector2f mousePos = g_window.mapPixelToCoords(sf::Mouse::getPosition(g_window));
			g_objectClass.deleteObject(mousePos, 0);
		}
}

void init(sf::RenderWindow& window, sf::Music& g_music)
{
	window.create(sf::VideoMode(800, 600), "Never Finite");

	sf::Image icon;
	if (!icon.loadFromFile("images/player.png"))
	{
		return;
	}
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	window.setFramerateLimit(60);

	if (!g_music.openFromFile("audio/music.ogg"))
	{
		return;
	}
	g_music.setLoop(true);
	g_music.play();
}

int main() {

	init(g_window, g_music);
	
	g_objectClass.createObject(64, 64, 0, 0, 255, 0, 0, 2, true);
	g_objectClass.createObject(400, 64, 0, 400, 118, 68, 166, 0, false);
	g_objectClass.applyTexture(g_PlayerArray[0], "images/player.png");
	createCoins(g_spawnRadius, g_coinLimit);

	while (g_window.isOpen())
	{
		while (g_window.pollEvent(g_event))
		{
			switch (g_event.type)
			{
			case sf::Event::Closed:
				g_window.close();
				break;
			}
			if (g_event.type == sf::Event::KeyPressed)
			handleKeyPressMain(g_event);
			if (g_event.type == sf::Event::KeyReleased)
			handleKeyReleaseMain(g_event);
		}

		handleMousePressMain();

		g_window.clear(sf::Color(48, 25, 52));

		if (!g_playerObjectCollides) {
			for (int i = 0; i < g_PlayerArray.size(); i++) { g_objectClass.applyGravity(g_PlayerArray[i], 5.0f, 5.0f); }
			g_playerObjectCollides = false;
		}

		for (int i = 0; i < g_PlayerArray.size(); i++) {
			g_text.setPosition(g_PlayerArray[i].getShape().getPosition().x + 250, g_PlayerArray[i].getShape().getPosition().y - 300);
			g_levelText.setPosition(g_PlayerArray[i].getShape().getPosition().x - 350, g_PlayerArray[i].getShape().getPosition().y - 300);
			if (g_moveLeft) g_objectClass.applyHorizontalMovement(g_PlayerArray[i], -4.5f, -5.0f);
			if (g_moveRight) g_objectClass.applyHorizontalMovement(g_PlayerArray[i], 4.5f, 5.0f);
		}

		g_playerObjectCollides = false;

		for (int i = 0; i < g_PlayerArray.size(); i++) { g_camera.viewFollow(g_PlayerArray[i].m_shape.getPosition(), g_window); }

		g_objectClass.checkCollision(g_PlayerArray, g_ColliderArray, g_objectClass, 3.0f, 3.0f, 3.0f, g_playerObjectCollides);

		updateDisplayedCoins(g_PlayerArray, g_InterActiveObjectArray, g_coinCount, g_text);

		for (const auto& collider : g_ColliderArray) { g_window.draw(collider.getShape()); }

		for (const auto& nonCollider : g_NonColliderArray) { g_window.draw(nonCollider.getShape()); }

		for (const auto& player : g_PlayerArray) { g_window.draw(player.getShape()); }

		for (const auto& interactiveObject : g_InterActiveObjectArray) { g_window.draw(interactiveObject.getShape()); }

		g_window.draw(g_text.getText());
		g_window.draw(g_levelText.getText());

		resetGameLoop(g_coinCount, g_camera, g_coinLimit, g_spawnRadius, g_levelCount, g_PlayerArray, g_ColliderArray, g_levelText, g_text, 50.0f, 300.0f, g_objectClass);

		for (int i = 0; i < g_PlayerArray.size(); i++)
		{
			g_PlayerArray[i].m_sprite.setPosition(g_PlayerArray[i].getShape().getPosition().x, g_PlayerArray[i].getShape().getPosition().y);
			g_window.draw(g_PlayerArray[i].m_sprite);
		}

		g_window.display();

	}
	return 0;
}
