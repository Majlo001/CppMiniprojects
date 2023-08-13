#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Obstacle {
private:
    Vector2f m_Position;
    RectangleShape m_Shape;
    bool m_Destroyed = false;
    bool m_Destroyable = true;

public:
    Obstacle(float, float, Color, bool);
    Obstacle(float, float);
    FloatRect getPosition();
    RectangleShape getShape();
    bool isDestroyed();
    void destroy();
    bool isDestroyable();
    void makeDestroyable(float);
};