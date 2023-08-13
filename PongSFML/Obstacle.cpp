#include "Obstacle.h"

Obstacle::Obstacle(float startX, float startY, Color color = Color::White, bool destroyable = true) {
    m_Position.x = startX;
    m_Position.y = startY;

    m_Shape.setSize(sf::Vector2f(100, 20));
    m_Shape.setPosition(m_Position);
    m_Shape.setFillColor(color);
    m_Destroyable = destroyable;
}

Obstacle::Obstacle(float startX, float startY) {
    m_Position.x = startX;
    m_Position.y = startY;

    m_Shape.setSize(sf::Vector2f(100, 20));
    m_Shape.setPosition(m_Position);
}

FloatRect Obstacle::getPosition() {
    return m_Shape.getGlobalBounds();
}

RectangleShape Obstacle::getShape() {
    return m_Shape;
}

bool Obstacle::isDestroyed() {
    return m_Destroyed;
}

void Obstacle::destroy() {
    m_Destroyed = true;
}

bool Obstacle::isDestroyable() {
    return m_Destroyable;
}

void Obstacle::makeDestroyable(float val) {
    m_Destroyable = val;
}