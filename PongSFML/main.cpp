#include<sstream>
#include<cstdlib>
#include<SFML/Graphics.hpp>

#include "Bat.h"
#include "Ball.h"


int main() {
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Pong", Style::Fullscreen);

    int score = 0;
    int lives = 3;

    Bat bat(1920/2, 1080-20);
    Ball ball(1920 / 2, 0);
	Text hud;

	
	Font font;
	font.loadFromFile("fonts/DS-DIGI.ttf");
	hud.setFont(font);
	hud.setCharacterSize(75);
	hud.setFillColor(Color::White);
	hud.setPosition(20, 20);

	Clock clock;

	while (window.isOpen()) {
		/*
		Handle the player input
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				// Quit the game when the window is closed
				window.close();

		}

		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle the pressing and releasing of the arrow keys
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			bat.moveLeft();
		}
		else
		{
			bat.stopLeft();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			bat.moveRight();
		}
		else
		{
			bat.stopRight();
		}

		/*
		Update the bat, the ball and the HUD
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		// Update the delta time
		Time dt = clock.restart();
		bat.update(dt);
        ball.update(dt);

		// Update the HUD text
		std::stringstream ss;
		ss << "Score:" << score << "    Lives:" << lives;
		hud.setString(ss.str());

        // Handle ball hitting the bottom
        if (ball.getPosition().top > window.getSize().y) {
            ball.reboundBottom();
            lives--;

            if (lives < 1) {
                score = 0;
                lives = 3;
            }
        }

        if (ball.getPosition().top < 0) {
            ball.reboundBatOrTop();
            score++;
        }

        if (ball.getPosition().left < 0 || ball.getPosition().left + ball.getPosition().width > window.getSize().x) {
            ball.reboundSides();
        }

        if (ball.getPosition().intersects(bat.getPosition())) {
            ball.reboundBatOrTop();
        }
		
		/*
		Draw the bat, the ball and the HUD
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/
		window.clear();
		window.draw(hud);
		window.draw(bat.getShape());
        window.draw(ball.getShape());
		window.display();
	}

    return 0;
}