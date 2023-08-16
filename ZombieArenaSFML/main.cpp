#include<SFML/Graphics.hpp>
#include<sstream>
#include<cstdlib>
#include<vector>

#include "Player.h"
#include "Main.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"


using namespace sf;


int main() {
    TextureHolder holder;

    enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	State state = State::GAME_OVER;

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;
	Time gameTimeTotal;

	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	Player player;
	IntRect arena;

    VertexArray background;
    Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

    int numZombies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    
    Time lastPressed;

    window.setMouseCursorVisible(true);
    Sprite spriteCrosshair;
    Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
    spriteCrosshair.setTexture(textureCrosshair);
    spriteCrosshair.setOrigin(25, 25);

    Pickup healthPickup(1);
    Pickup ammoPickup(2);

    int score = 0;
    int hiScore = 0;

    while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				}

				else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					clock.restart();
				}

				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state = State::LEVELING_UP;
				}


				if (state == State::PLAYING) {
                    // Reloading
                    if (event.key.code == Keyboard::R) {
                        if (bulletsSpare >= clipSize) {
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                        }
                        else if (bulletsSpare > 0) {
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                        }
                        else {
                            // More here later
                        }
                    }
				}

			}
		}


		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (state == State::PLAYING) {
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else {
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else {
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else {
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else {
				player.stopRight();
			}

            // Fire a bullet
            if (Mouse::isButtonPressed(Mouse::Left)) {
                if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0) {
                    bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
                    currentBullet++;

                    if (currentBullet > 99) {
                        currentBullet = 0;
                    }

                    lastPressed = gameTimeTotal;
                    bulletsInClip--;
                }
            }
		}


		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5) {
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6) {
				state = State::PLAYING;
			}
			
			if (state == State::PLAYING) {
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

                int tileSize = createBackground(background, arena);
				player.spawn(arena, resolution, tileSize);
				
                healthPickup.setArena(arena);
                ammoPickup.setArena(arena);

                numZombies = 10;
                delete[] zombies;
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;

                clock.restart();
			}
		}




		if (state == State::PLAYING) {
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

            spriteCrosshair.setPosition(mouseWorldPosition);
			player.update(dtAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());		
			mainView.setCenter(player.getCenter());

            for (int i = 0; i < numZombies; i++) {
                if (zombies[i].isAlive()) {
                    zombies[i].update(dtAsSeconds, playerPosition);
                }
            }

            for (int i = 0; i < 100; i++) {
                if (bullets[i].isInFlight()) {
                    bullets[i].update(dtAsSeconds);
                }
            }

            healthPickup.update(dtAsSeconds);
            ammoPickup.update(dtAsSeconds);

            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < numZombies; j++) {
                    if (bullets[i].isInFlight() && zombies[j].isAlive()) {
                        if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
                            bullets[i].stop();

                            if (zombies[j].hit()) {
                                score += 10;
                                if (score >= hiScore) {
                                    hiScore = score;
                                }

                                numZombiesAlive--;

                                if (numZombiesAlive == 0) {
                                    state = State::LEVELING_UP;
                                }

                                // if (zombies[j].getType() == 0) {
                                //     healthPickup.setArena(arena);
                                //     healthPickup.spawn();
                                // }
                                // else if (zombies[j].getType() == 1) {
                                //     ammoPickup.setArena(arena);
                                //     ammoPickup.spawn();
                                // }
                            }
                        }
                    }
                }
            }

            for (int i = 0; i < numZombies; i++) {
                if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()) {
                    if (player.hit(gameTimeTotal)) {
                        // More here later
                    }

                    if (player.getHealth() <= 0) {
                        state = State::GAME_OVER;
                    }
                }
            }

            if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
                player.increaseHealthLevel(healthPickup.gotIt());
            }
            
            if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
                bulletsSpare += ammoPickup.gotIt();
            }

		}


		if (state == State::PLAYING) {
			window.clear();
			window.setView(mainView);
            window.draw(background, &textureBackground);

            for (int i = 0; i < numZombies; i++) {
                window.draw(zombies[i].getSprite());
            }

            for (int i = 0; i < 100; i++) {
                if (bullets[i].isInFlight()) {
                    window.draw(bullets[i].getShape());
                }
            }

			window.draw(player.getSprite());

            if (ammoPickup.isSpawned()) {
                window.draw(ammoPickup.getSprite());
            }
            
            if (healthPickup.isSpawned()) {
                window.draw(healthPickup.getSprite());
            }

            window.draw(spriteCrosshair);
		}

		// if (state == State::LEVELING_UP)
		// {
		// }

		// if (state == State::PAUSED)
		// {
		// }

		// if (state == State::GAME_OVER)
		// {
		// }

		window.display();
	}

    delete[] zombies;
    return 0;
}


Zombie* createHorde(int numZombies, IntRect arena) {
    Zombie* zombies = new Zombie[numZombies];

    int maxY = arena.height - 20;
    int minY = arena.top + 20;
    int maxX = arena.width - 20;
    int minX = arena.left + 20;

    for (int i = 0; i < numZombies; i++) {
        srand((int)time(0) * i);
        int side = (rand() % 4);
        float x, y;

        switch (side) {
            case 0:
                x = minX;
                y = (rand() % maxY) + minY;
                break;
            case 1:
                x = maxX;
                y = (rand() % maxY) + minY;
                break;
            case 2:
                x = (rand() % maxX) + minX;
                y = minY;
                break;
            case 3:
                x = (rand() % maxX) + minX;
                y = maxY;
                break;
        }

        srand((int)time(0) * i * 2);
        int type = (rand() % 3);

        zombies[i].spawn(x, y, type, i);
    }

    return zombies;
}