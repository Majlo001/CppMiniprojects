#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

void Engine::update(float dtAsSeconds) {
    if (m_NewLevelRequired) {
        loadLevel();
    }

	if (m_Playing) {
        m_Thomas.update(dtAsSeconds);
        m_Bob.update(dtAsSeconds);


        if (detectCollisions(m_Thomas) && detectCollisions(m_Bob)) {
            m_NewLevelRequired = true;
        }
        else {
            detectCollisions(m_Bob);
        }

        if (m_Bob.getFeet().intersects(m_Thomas.getHead())) {
            m_Bob.stopFalling(m_Thomas.getHead().top);
        }
        else if (m_Thomas.getFeet().intersects(m_Bob.getHead())) {
            m_Thomas.stopFalling(m_Bob.getHead().top);
        }


		m_TimeRemaining -= dtAsSeconds;

		if (m_TimeRemaining <= 0) {
			m_NewLevelRequired = true;
		}
	}

    if (m_SplitScreen) {
        m_LeftView.setCenter(m_Thomas.getCenter());
        m_RightView.setCenter(m_Bob.getCenter());
    }
    else {
        if (m_Character1) {
            m_MainView.setCenter(m_Thomas.getCenter());
        }
        else {
            m_MainView.setCenter(m_Bob.getCenter());
        }
    }
}