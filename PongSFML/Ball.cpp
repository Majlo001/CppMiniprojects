#include "Ball.h"

Ball::Ball(float startX, float startY) {
    m_Position.x = startX;
    m_Position.y = startY;

    m_Shape.setSize(sf::Vector2f(20, 20));
    m_Shape.setPosition(m_Position);
}

FloatRect Ball::getPosition() {
    return m_Shape.getGlobalBounds();
}

RectangleShape Ball::getShape() {
    return m_Shape;
}

float Ball::getXVelocity() {
    return m_DirectionX;
}

float Ball::getYVelocity() {
    return m_DirectionY;
}

Vector2f Ball::getDirection() {
    return Vector2f(m_DirectionX, m_DirectionY);
}

void Ball::reboundSides() {
    m_DirectionX = -m_DirectionX;
}

void Ball::reboundBatOrTop() {
    m_DirectionY = -m_DirectionY;
}

void Ball::reboundBottom() {
    m_Position.y = 20;
    m_Position.x = 500;
    m_DirectionY = -m_DirectionY;
}

void Ball::update(Time dt) {
    m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
    m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();

    m_Shape.setPosition(m_Position);
}


void Ball::updateSpeed(float value) {
    m_Speed *= value;
}

void Ball::restartSpeed() {
    m_Speed = 500.0f;
}