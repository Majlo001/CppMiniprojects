#include<SFML/Graphics.hpp>
#include<sstream>
#include<cstdlib>
#include<vector>

#include "Bat.h"
#include "Ball.h"
#include "Obstacle.h"


int main() {
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Pong", Style::Fullscreen);

    Vector2u windowSize = window.getSize();
    unsigned int windowWidth = windowSize.x;
    unsigned int windowHeight = windowSize.y;

    int score = 0;
    int lives = 3;

    int speedMultiplier = 1; 

    Bat bat(windowWidth/2, windowHeight-20);
    Ball ball(windowWidth/2, 0);
	Text hud;


    std::vector<std::vector<Obstacle>> obstaclesMatrix;
    for (int row = 0; row < 5; ++row) {
        obstaclesMatrix.emplace_back();
        for (int col = 0; col < 6; ++col) {
            obstaclesMatrix[row].emplace_back(200 + col * 300, 300 + row * 100);
        }
    }
    for (int row = 5; row < 6; ++row) {
        obstaclesMatrix.emplace_back();
        for (int col = 0; col < 6; ++col) {
            obstaclesMatrix[row].emplace_back(200 + col * 300, 300 + row * 100, Color::Yellow, false);
        }
    }

	
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
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

		}

		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		// Handle the pressing and releasing of the arrow keys
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			bat.moveLeft();

            if (bat.getPosition().left < 0) {
                bat.stopLeft();
            }
		}
		else {
			bat.stopLeft();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			bat.moveRight();

            if (bat.getPosition().left + bat.getPosition().width > windowWidth) {
                bat.stopRight();
            }
		}
		else {
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
                ball.restartSpeed();
                score = 0;
                lives = 3;
                speedMultiplier = 1;
            }
        }

        if (ball.getPosition().top <= 0) {
            ball.reboundBatOrTop();
            score++;
        }

        if (ball.getPosition().left < 0 || ball.getPosition().left + ball.getPosition().width > window.getSize().x) {
            ball.reboundSides();
        }

        if (ball.getPosition().intersects(bat.getPosition())) {
            FloatRect ballBounds = ball.getPosition();
            FloatRect batBounds = bat.getPosition();

            float ballCenterX = ballBounds.left + ballBounds.width / 2;
            float batCenterX = batBounds.left + batBounds.width / 2;

            float offsetX = ballCenterX - batCenterX;
            float offsetY = ballBounds.top - batBounds.top;

            
            if (offsetX < -batBounds.width / 4) { // Odbicie w lewo
                ball.reboundSides();
                ball.reboundBatOrTop();
            } 
            else if (offsetX > batBounds.width / 4) { // Odbicie w prawo
                ball.reboundSides();
                ball.reboundBatOrTop();
            } 
            else {
                ball.reboundBatOrTop();
            }
        }


        for (auto& row : obstaclesMatrix) {
            for (Obstacle& obstacle : row) {
                if (ball.getPosition().intersects(obstacle.getPosition())) {
                    FloatRect ballBounds = ball.getPosition();
                    FloatRect obstacleBounds = obstacle.getPosition();

                    float ballRight = ballBounds.left + ballBounds.width;
                    float ballBottom = ballBounds.top + ballBounds.height;
                    float obstacleRight = obstacleBounds.left + obstacleBounds.width;
                    float obstacleBottom = obstacleBounds.top + obstacleBounds.height;

                    bool ballFromTop = ballBottom > obstacleBounds.top && ballBounds.top < obstacleBounds.top;
                    bool ballFromBottom = ballBounds.top < obstacleBottom && ballBottom > obstacleBottom;
                    bool ballFromLeft = ballRight > obstacleBounds.left && ballBounds.left < obstacleBounds.left;
                    bool ballFromRight = ballBounds.left < obstacleRight && ballRight > obstacleRight;

                    if (obstacle.isDestroyable() == true) {
                        score += 10;
                        obstacle.destroy();
                        obstacle = Obstacle(-100, -100);
                    }
                    
                    if ((ballFromTop && ball.getDirection().y > 0) || (ballFromBottom && ball.getDirection().y < 0)) {
                        ball.reboundBatOrTop();
                    }

                    if ((ballFromLeft && ball.getDirection().x > 0) || (ballFromRight && ball.getDirection().x < 0)) {
                        ball.reboundSides();
                    }

                }
            }
        }

        

        if (score >= 30 * speedMultiplier) {
            ball.updateSpeed(1.1f);
            speedMultiplier++;
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

        for (auto& row : obstaclesMatrix) {
            for (Obstacle& obstacle : row) {
                window.draw(obstacle.getShape());
            }
        }

		window.display();
	}

    return 0;
}