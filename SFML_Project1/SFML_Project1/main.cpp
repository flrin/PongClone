#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string>

using namespace sf;
using namespace std;

const float ballSpeed = 35;

float getNewXSpeed(float oldX, float newY) {
	//cout << "oldX: " << oldX << endl << "oldY: " << oldY << endl << "oldSpeed: " << oldSpeed << endl;

	if (ballSpeed - newY * newY < 0) {
		cout << "newX: " << sqrt(-1 * (ballSpeed - newY * newY)) << endl << "newY: " << newY << endl << "newSpeed: " << sqrt(-1 * (ballSpeed - newY * newY)) * sqrt(-1 * (ballSpeed - newY * newY)) + newY * newY << endl;
		return sqrt(-1 * (ballSpeed - newY * newY));
	}
	else {
		cout << "newX: " << sqrt(ballSpeed - newY * newY) << endl << "newY: " << newY << endl << "newSpeed: " << sqrt(ballSpeed - newY * newY) * sqrt(ballSpeed - newY * newY) + newY * newY << endl;
		return sqrt(ballSpeed - newY * newY);
	}
}

void SetVertexArrayBox(VertexArray& box, float height, float width, Vector2f originPos) {
	box[0].position = Vector2f(originPos.x,originPos.y);
	box[1].position = Vector2f(originPos.x+width,originPos.y);
	box[2].position = Vector2f(originPos.x+width,originPos.y+height);
	box[3].position = Vector2f(originPos.x,originPos.y+height);
	box[4].position = Vector2f(originPos.x,originPos.y);
}

class GameState {

public:
	enum GameType {
		GameRunning,
		MainMenu,
		EndMenu,

		count
	};

	GameType type;

};

FloatRect getBoundingBoxofBox(VertexArray box) {
	
	return FloatRect(box[0].position.x, box[0].position.y, box[1].position.x - box[0].position.x, box[3].position.y - box[0].position.y);
}

int main()
{
	GameState gameState;
	gameState.type = GameState::MainMenu;
	RenderWindow window(VideoMode(1300, 900), "Pong");
	window.setVerticalSyncEnabled(1);

	float playerSize = 150, ballSize = 15;
	int s1 = 0, s2 = 0;

	VertexArray linieMijloc(Quads, 100);
	int k = 0;
	for (int i = 900; i >= 0; i -= 51) {

		linieMijloc[k].position = Vector2f(643, i);
		linieMijloc[k].color = Color::White;
		k++;

		linieMijloc[k].position = Vector2f(657, i);
		linieMijloc[k].color = Color::White;
		k++;

		linieMijloc[k].position = Vector2f(657, i - 30);
		linieMijloc[k].color = Color::White;
		k++;

		linieMijloc[k].position = Vector2f(643, i - 30);
		linieMijloc[k].color = Color::White;
		k++;

	}

	Font font;
	font.loadFromFile("bit5x3.ttf");
	Text scor1;
	Text scor2;
	scor1.setFont(font);
	scor2.setFont(font);
	scor1.setPosition(300, 0);
	scor2.setPosition(1000, 0);
	scor1.setCharacterSize(100);
	scor2.setCharacterSize(100);

	RectangleShape p1(Vector2f(30, playerSize));
	RectangleShape p2(Vector2f(30, playerSize));
	p1.setOrigin(15, playerSize / 2);
	p2.setOrigin(15, playerSize / 2);
	p1.setPosition(45, 450);
	p2.setPosition(1255, 450);

	CircleShape ball(ballSize);
	ball.setOrigin(ballSize, ballSize);
	ball.setPosition(Vector2f(650, 450));

	float dx = 0.f, dy = 0.f, bx, by, playerSpeed = 4.56f, offset = 23;
	dx = 5;
	dy = 0;

	bool bStat = 1, servingSide = 0;

	Clock clock;
	Time t1 = seconds(1);
	Event event;
	bool canPress = 0;

	while (window.isOpen()) {
		switch (gameState.type) {
			case GameState::GameRunning: {
				

				FloatRect ballBox = ball.getGlobalBounds();
				FloatRect p1Box = p1.getGlobalBounds();
				FloatRect p2Box = p2.getGlobalBounds();




				bx = ball.getPosition().x;
				by = ball.getPosition().y;
				while (window.pollEvent(event)) {
					switch (event.type) {
					case Event::Closed:
						window.close();
					}
					}
					if (Keyboard::isKeyPressed(Keyboard::A) && p1.getPosition().y > playerSize / 2) {
						p1.move(Vector2f(0, -playerSpeed));
					}

					if (Keyboard::isKeyPressed(Keyboard::D) && p1.getPosition().y < 900 - playerSize / 2) {
						p1.move(Vector2f(0, playerSpeed));

					}
					if (Keyboard::isKeyPressed(Keyboard::Up) && p2.getPosition().y > playerSize / 2) {
						p2.move(Vector2f(0, -playerSpeed));
					}
					if (Keyboard::isKeyPressed(Keyboard::Down) && p2.getPosition().y < 900 - playerSize / 2) {
						p2.move(Vector2f(0, playerSpeed));
					}

					if (ballBox.intersects(p1Box) && !bStat) {
						bStat = 1;
						dy = (ball.getPosition().y - p1.getPosition().y) / offset;
						dx = getNewXSpeed(dx, dy);
						cout << endl;
					}
					if (ballBox.intersects(p2Box) && bStat) {
						bStat = 0;
						dy = (ball.getPosition().y - p2.getPosition().y) / offset;
						dx = -1 * getNewXSpeed(dx, dy);
						cout << endl;
					}
					if (by > 900 - ballSize) {
						dy = -dy;
					}
					if (by < ballSize) {
						dy = -dy;
					}
					if (bx > 1300 || bx < 0) {
						dy = 0;
						if (servingSide) {
							dx = 5;
							servingSide = 0;
							bStat = 1;
						}
						else {
							dx = -5;
							servingSide = 1;
							bStat = 0;
						}
						if (bx > 1300)s1++;
						else s2++;

						ball.setPosition(650, 450);
						clock.restart();
					}

					t1 = clock.getElapsedTime();

					scor1.setString(to_string(s1));
					scor2.setString(to_string(s2));

					window.clear();

					window.draw(linieMijloc);
					window.draw(scor1);
					window.draw(scor2);
					window.draw(p1);
					window.draw(p2);
					if (t1.asMilliseconds() >= 1000) {
						ball.move(Vector2f(dx, dy));
						window.draw(ball);
					}

					window.display();

					if (s1 == 10 || s2 == 10)
						gameState.type = GameState::EndMenu;
			}
			break;

			case GameState::MainMenu: {

				Text gameTitle;
				gameTitle.setFont(font);
				gameTitle.setString("PONG");
				gameTitle.setPosition(370, 20);
				gameTitle.setCharacterSize(300);

				Text multyPlayerText;
				multyPlayerText.setFont(font);
				multyPlayerText.setString("Multyplayer");
				multyPlayerText.setPosition(500, 400);
				multyPlayerText.setCharacterSize(50);

				VertexArray mpBox(LineStrip, 5);
				SetVertexArrayBox(mpBox, 100, 300, Vector2f(485, 390));

				FloatRect mpBoxBoundingBox = getBoundingBoxofBox(mpBox);

				while (window.pollEvent(event)) {
					if (event.type == Event::KeyPressed) {
						if (event.key.code == Keyboard::F) {
							gameState.type = GameState::GameRunning;
						}
					}
					if (event.type == Event::Closed) {
						window.close();
					}
					if (event.type == Event::MouseButtonPressed) {
						Vector2i point = Mouse::getPosition(window);

						if (mpBoxBoundingBox.contains(Vector2f(point.x, point.y))) {
							canPress = 1;
						}
						
					}
					if (event.type == Event::MouseButtonReleased) {
						Vector2i point = Mouse::getPosition(window);

						if (mpBoxBoundingBox.contains(Vector2f(point.x, point.y))&&canPress) {
							gameState.type = GameState::GameRunning;
						}
						canPress = 0;
					}

					window.clear();

					window.draw(gameTitle);
					window.draw(multyPlayerText);
					window.draw(mpBox);

					window.display();
				}
			}
			break;
		
			case GameState::EndMenu: {
				s1 = 0;
				s2 = 0;

				Text gameOverText;
				gameOverText.setFont(font);
				gameOverText.setString("GAME OVER");
				gameOverText.setPosition(200, 20);
				gameOverText.setCharacterSize(200);

				Text playAgainText;
				playAgainText.setFont(font);
				playAgainText.setString("Play Again?");
				playAgainText.setPosition(500, 360);
				playAgainText.setCharacterSize(50);

				Text yesText,noText;
				yesText.setFont(font);
				yesText.setString("YES");
				yesText.setPosition(500,500);
				yesText.setCharacterSize(50);
				noText.setFont(font);
				noText.setString("NO");
				noText.setPosition(700, 500);
				noText.setCharacterSize(50);

				VertexArray yesBox(LineStrip, 5), noBox(LineStrip, 5);
				SetVertexArrayBox(yesBox, 50, 100, Vector2f(485, 510));
				SetVertexArrayBox(noBox, 50, 100, Vector2f(675, 510));

				FloatRect yesBoxBB = getBoundingBoxofBox(yesBox);
				FloatRect noBoxBB = getBoundingBoxofBox(noBox);

				while (window.pollEvent(event)) {
					if (event.type == Event::Closed) {
						window.close();
					}
					if (event.type == Event::MouseButtonPressed) {
						Vector2i point = Mouse::getPosition(window);

						if (yesBoxBB.contains(Vector2f(point.x, point.y))) {
							gameState.type = GameState::GameRunning;
						}
						if (noBoxBB.contains(Vector2f(point.x, point.y))) {
							window.close();
						}

					}
					window.clear();

					window.draw(gameOverText);
					window.draw(playAgainText);
					window.draw(yesText);
					window.draw(noText);
					window.draw(yesBox);
					window.draw(noBox);
	
					window.display();
				}

				
			}
			break;
		}
	}

	return 0;
}